#ifndef __lxthunk__
#define __lxthunk__

#include "lxstack.hpp"
#include <exception>

// C++ function calling with Lua stack interface using variadic templates
// There are 4 cases: with/without return value, with/without class object

template <typename... Args>
 inline int lux_thunk(lua_State *state, void fun(Args...))
 {
	try {
	register int arg = 0;
	fun(lux_to<Args>(state, --arg)...);
	return 0;
	}
	catch (std::exception &error)
	{
	return luaL_error(state, "C++ exception: %s", error.what());
	}
 }

template <typename Res, typename... Args>
 inline int lux_thunk(lua_State *state, Res fun(Args...))
 {
	try {
	register int arg = 0;
	lux_push<Res>(state, fun(lux_to<Args>(state, --arg)...));
	return 1;
	}
	catch (std::exception &error)
	{
	return luaL_error(state, "C++ exception: %s", error.what());
	}
 }

template <typename Obj, typename... Args>
 inline int lux_thunk(lua_State *state, void (Obj::*fun)(Args...))
 {
	try {
	register int arg = 0;
	const int top = 1 + sizeof...(Args);
	auto obj = lux_to<Obj*>(state, -top);
	(obj->*fun)(lux_to<Args>(state, --arg)...);
	return 0;
	}
	catch (std::exception &error)
	{
	return luaL_error(state, "C++ exception: %s", error.what());
	}
 }

template <typename Res, typename Obj, typename... Args>
 inline int lux_thunk(lua_State *state, Res (Obj::*fun)(Args...))
 {
	try {
	register int arg = 0;
	const int top = 1 + sizeof...(Args);
	auto obj = lux_to<Obj*>(state, -top);
	lux_push<Res>(state, (obj->*fun)(lux_to<Args>(state, --arg)...));
	return 1;
	}
	catch (std::exception &error)
	{
	return luaL_error(state, "C++ exception: %s", error.what());
	}
 }

// Wrap the above thunks in an anonymous function compatible with Lua

#define lux_wrap(fun) [](lua_State *state)->int{return lux_thunk(state, &fun);}

// Cast the wrapped C++ function to Lua's "C" function

#define lux_cast(fun) static_cast<lua_CFunction>(lux_wrap(fun))


#endif // file
