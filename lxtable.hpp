#ifndef __lxtable__
#define __lxtable__

#include "lxstack.hpp"

// Tabular versions of the push/to functions defined elsewhere

template <class User>
User lux_getfield(lua_State *state, int stack, const char *key)
{
	lua_getfield(state, stack, key);
	User data = lux_to<User>(state, -1);
	lua_pop(state, 1);
	return data;
}

template <class User>
void lux_setfield(lua_State *state, int stack, const char *key, User data)
{
	lux_push(state, data);
	lua_setfield(state, stack, key);
}

// Shorthand for manipulating Lua tables

struct lux_Table
{
	lua_State *state;
	int stack;

	lux_Table(lua_State *state, int stack=0)
	{
		if (!stack)
		{
			lua_newtable(state);
			stack = lua_gettop(state);
		}
		this->state = state;
		this->stack = stack;
	}

	template <class Base> Base get(const char *key)
	{
		return lux_getfield<Base>(state, stack, key);
	}

	template <class Base> void set(const char *key, Base value)
	{
		lux_setfield<Base>(state, stack, key, value);
	}
};

// For manipulating Lua tables as arguments to C++ functions

template <> inline lux_Table lux_to<lux_Table>(lua_State *state, int stack)
{
	luaL_checktype(state, stack, LUA_TTABLE);
	return lux_Table(state, stack);
}

#endif // file

