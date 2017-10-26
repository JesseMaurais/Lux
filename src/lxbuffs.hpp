#ifndef __lxbuffs__
#define __lxbuffs__

#include "lua.hpp"

// Just a wrapper around the Lua C buffer for syntactic sugar

struct lux_Buffer : luaL_Buffer
{
	char *string;

	lux_Buffer(lua_State *state)
	{
		luaL_buffinit(state, this);
	}

	lux_Buffer(lua_State *state, std::size_t size)
	{
		string = luaL_buffinitsize(state, this, size);
	}

	void addchar(char c)
	{
		luaL_addchar(this, c);
	}

	void addstring(const char *s)
	{
		luaL_addstring(this, s);
	}

	void addstring(const char *s, std::size_t n)
	{
		luaL_addlstring(this, s, n);
	}

	void addsize(std::size_t n)
	{
		luaL_addsize(this, n);
	}

	void addvalue(void)
	{
		luaL_addvalue(this);
	}

	char *prep(void)
	{
		return luaL_prepbuffer(this);
	}

	char *prep(std::size_t n)
	{
		return luaL_prepbuffsize(this, n);
	}

	void push(void)
	{
		luaL_pushresult(this);
	}

	void push(std::size_t n)
	{
		luaL_pushresultsize(this, n);
	}
};

#endif // file

