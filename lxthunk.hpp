#ifndef __lxthunk__
#define __lxthunk__

#include "lxstack.hpp"

// C++ function calling with Lua stack interface using variadic templates
// There are 4 cases: with/without return value, with/without class object

template <typename... Args>
 inline int lux_thunk(lua_State *state, void fun(Args...))
 {
	register int arg = 0;
	fun(lux_to<Args>(state, --arg)...);
	return 0;
 }

template <typename Res, typename... Args>
 inline int lux_thunk(lua_State *state, Res fun(Args...))
 {
	register int arg = 0;
	lux_push<Res>(state, fun(lux_to<Args>(state, --arg)...));
	return 1;
 }

template <typename Obj, typename... Args>
 inline int lux_thunk(lua_State *state, void (Obj::*fun)(Args...))
 {
	register int arg = 0;
	const int top = 1 + sizeof...(Args);
	auto obj = lux_to<Obj*>(state, -top);
	(obj->*fun)(lux_to<Args>(state, --arg)...);
	return 0;
 }

template <typename Res, typename Obj, typename... Args>
 inline int lux_thunk(lua_State *state, Res (Obj::*fun)(Args...))
 {
	register int arg = 0;
	const int top = 1 + sizeof...(Args);
	auto obj = lux_to<Obj*>(state, -top);
	lux_push<Res>(state, (obj->*fun)(lux_to<Args>(state, --arg)...));
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
