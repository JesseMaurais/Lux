#ifndef __lxtable__
#define __lxtable__

#include "lxstack.hpp"

// To set or get data members from a field in a table at stack index

template <class User>
void lux_setfield(lua_State *state, int stack, const char *key, User data)
{
	lux_push(state, data);
	lua_setfield(state, stack, key);
}

template <class User>
User lux_getfield(lua_State *state, int stack, const char *key)
{
	lua_getfield(state, stack, key);
	User data = lux_to<User>(state, -1);
	lua_pop(state, 1);
	return data;
}

// Similar to luaL_Reg but for all classes for which lux_push is defined

template <class User> struct lux_Reg
{
	const char *name;
	User value;
};

template <class User>
void lux_settable(lua_State *state, const lux_Reg<User> regs[])
{
	for (auto r=regs; r->name; ++r)
	{
		lua_pushstring(state, r->name);
		lux_push<User>(state, r->value);
		lua_settable(state, -3);
	}
}

// Shorthand for manipulating tables as C function arguments

struct lux_Table
{
	lua_State *state;
	int stack;

	lux_Table(lua_State *state, int stack=-1)
	{
		this->state = state;
		this->stack = stack;
	}

	template <class User> void set(const char *key, User data)
	{
		lux_setfield<User>(state, stack, key, data);
	}

	template <class User> User get(const char *key)
	{
		return lux_getfield<User>(state, stack, key);
	}
};

template <> inline
lux_Table lux_to<lux_Table>(lua_State *state, int stack)
{
	return lux_Table(state, stack);
}

#endif // file

