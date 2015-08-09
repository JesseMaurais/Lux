#ifndef __lxtools__
#define __lxtools__

/*
 * This is where I'm putting new ideas that don't seem to fit anywhere else.
 */

#include "lxstack.hpp"
#include <cstring>

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

#endif // file

