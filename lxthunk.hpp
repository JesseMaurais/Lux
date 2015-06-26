#ifndef __lxthunk__
#define __lxthunk__

#include "lxalloc.hpp"
#include "lxstack.hpp"
#include <exception>

typedef int lux_CFunction(lua_State *state);

// C++ function calling with Lua stack interface using variadic templates
// there are 4 cases: with/without return value, with/without class object

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

// a special 5th case is that of constructors with arguments

template <typename Obj, typename... Args>
 inline int lux_thunk(lua_State *state)
 {
	typedef lux_Type<Obj> Type;
	try {
	register int arg = 0;
	new (state) Type(new Obj (lux_to<Args>(state, --arg)...), 1);
	return 1;
	}
	catch (std::exception &error)
	{
	return luaL_error(state, "C++ exception: %s", error.what());
	}
 }

// wrap the above thunks in an anonymous function compatible with Lua
#define lux_wrap(fun) [](lua_State *state)->int{return lux_thunk(state, &fun);}

// cast the wrapped C++ function to a Lua cfunction
#define lux_cast(fun) static_cast<lua_CFunction>(lux_wrap(fun))

// example of how lux_cast can be used anywhere cfunctions are expected
#define lux_register(state, fun) lua_register(state, #fun, lux_cast(fun))

#endif // file
