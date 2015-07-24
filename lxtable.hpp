#ifndef __lxtable__
#define __lxtable__

#include "lxstack.hpp"

// Similar to luaL_Reg but for all classes for which lux_push is defined

template <class Base> struct lux_Reg
{
	const char *name;
	Base value;
};

template <class Base>
void lux_settable(lua_State *state, const lux_Reg<Base> regs[])
{
	for (auto r=regs; r->name; ++r)
	{
		lua_pushstring(state, r->name);
		lux_push<Base>(state, r->value);
		lua_settable(state, -3);
	}
}

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
		lux_setfield(state, stack, key, data);
	}

	template <class user> User get(const char *key)
	{
		return lux_getfield(state, stack, key);
	}
};

template <> inline
lux_Table lux_to<lux_Table>(lua_State *state, int stack)
{
	return lux_Table(state, stack);
}

#endif // file

