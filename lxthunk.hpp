#ifndef __lxthunk__
#define __lxthunk__

/**
 * C++ function calling with Lua stack interface using variadic templates. We
 * automatically push/to values on the Lua stack given the arity of functions
 * in C++ (that is, their number of arguments and the argument types) as well
 * as the return value, if the functions have one. There are exactly 4 cases:
 * functions with/without return values, and ones with/without class objects.
 * Additionally, we wrap these in another function which will call the others
 * but will catch any standard C++ exceptions and report them to Lua, so that
 * ther interpreter can perform a stack trace. Finally, a preprocessing macro
 * is provided to simply "cast" an arbitrary C++ function, which really means
 * wrapping it again in an anonymous (lambda) function. All the wrapping will
 * be inlined away at compile time. 
 */

#include "lxstack.hpp"

template <typename... Args> inline
int lux_thunk(lua_State *state, void fun(Args...))
{
	register size_t arg = sizeof...(Args);
	fun(lux_to<Args>(state, arg--)...);
	return 0;
}

template <typename Res, typename... Args> inline
int lux_thunk(lua_State *state, Res fun(Args...))
{
	register size_t arg = sizeof...(Args);
	lux_push<Res>(state, fun(lux_to<Args>(state, arg--)...));
	return 1;
}

template <typename Obj, typename... Args> inline
int lux_thunk(lua_State *state, void (Obj::*fun)(Args...))
{
	register size_t arg = 1 + sizeof...(Args);
	auto obj = lux_to<Obj*>(state, 1);
	(obj->*fun)(lux_to<Args>(state, arg--)...);
	return 0;
}

template <typename Res, typename Obj, typename... Args> inline
int lux_thunk(lua_State *state, Res (Obj::*fun)(Args...))
{
	register size_t arg = 1 + sizeof...(Args);
	auto obj = lux_to<Obj*>(state, 1);
	lux_push<Res>(state, (obj->*fun)(lux_to<Args>(state, arg--)...));
	return 1;
}

// Exception safe version of the above

#include <exception>

template <typename Function>
 inline int lux_nothunk(lua_State *state, Function fun) 
 {
	try
	{
	return lux_thunk(state, fun);
	}
	catch (std::exception &error)
	{
	return luaL_error(state, "C++ exception: %s", error.what());
	}
	catch (...)
	{
	return luaL_error(state, "C++ exception: ...");
	}
 }

// Wrap the above thunks in an anonymous function compatible with Lua

#define lux_cast(fun) \
	[](lua_State *state)->int{ return lux_nothunk(state, &fun); }


#endif // file
