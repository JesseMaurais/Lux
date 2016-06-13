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
 * it's interpreter can perform a stack trace. Finally, a preprocessing macro
 * is provided to simply "cast" an arbitrary C++ function, which really means
 * wrapping it again in an anonymous (lambda) function. All the wrapping will
 * be inlined away at compile time. 
 */

#include "lxstack.hpp"
#include <iostream>
#include <utility>

// Case 1: basic function with no return value

template <typename... Args, std::size_t... Index> inline
int lux_thunk(lua_State *state, void fun(Args...), std::index_sequence<Index...>)
{
	fun(lux_to<Args>(state, Index+1)...);
	return 0;
}
template <typename... Args> inline
int lux_thunk(lua_State *state, void fun(Args...))
{
	return lux_thunk(state, fun, std::index_sequence_for<Args...>{});
}

// Case 2: basic function has return value

template <typename Res, typename... Args, std::size_t... Index> inline
int lux_thunk(lua_State *state, Res fun(Args...), std::index_sequence<Index...>)
{
	return lux_push<Res>(state, fun(lux_to<Args>(state, Index+1)...));
}
template <typename Res, typename... Args> inline
int lux_thunk(lua_State *state, Res fun(Args...))
{
	return lux_thunk(state, fun, std::index_sequence_for<Args...>{});
}

// Case 3: class function with no return value

template <typename Obj, typename... Args, std::size_t... Index> inline
int lux_thunk(lua_State *state, void (Obj::*fun)(Args...), std::index_sequence<Index...>)
{
	auto obj = lux_to<Obj*>(state, 1);
	(obj->*fun)(lux_to<Args>(state, Index+2)...);
	return 0;
}
template <typename Obj, typename... Args> inline
int lux_thunk(lua_State *state, void (Obj::*fun)(Args...))
{
	return lux_thunk(state, fun, std::index_sequence_for<Args...>());
}

// Case 4: class function has return value

template <typename Res, typename Obj, typename... Args, std::size_t... Index> inline
int lux_thunk(lua_State *state, Res (Obj::*fun)(Args...), std::index_sequence<Index...>)
{
	auto obj = lux_to<Obj*>(state, 1);
	return lux_push<Res>(state, (obj->*fun)(lux_to<Args>(state, Index+2)...));
}
template <typename Res, typename Obj, typename... Args> inline
int lux_thunk(lua_State *state, Res (Obj::*fun)(Args...))
{
	return lux_thunk(state, fun, std::index_sequence_for<Args...>());
}

// Exception safe version of the above

#include <exception>

template <typename Function> inline
int lux_nothunk(lua_State *state, Function fun) noexcept 
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
