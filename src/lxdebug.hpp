#ifndef __lxdebug__
#define __lxdebug__

#include "lua.hpp"

struct lux_Debug : lua_Debug
{
	bool getstack(lua_State *state, int level)
	{
		return lua_getstack(state, level, this);
	}

	const char *getlocal(lua_State *state, int n)
	{
		return lua_getlocal(state, this, n);
	}

	bool getinfo(lua_State *state, const char *what="nSlut")
	{
		return lua_getinfo(state, what, this);
	}
};

#endif // file
