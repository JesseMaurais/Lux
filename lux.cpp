#include "lux.hpp"


// LXALLOC.HPP ////////////////////////////////////////////////////////////////


void *operator new (size_t size, lua_State *state)
{
	return lua_newuserdata(state, size);
}

void *operator new [] (size_t size, lua_State *state)
{
	return lua_newuserdata(state, size);
}


// LXUNION.HPP ////////////////////////////////////////////////////////////////


#include <cstdlib>
#include <cstring>

static int compare(const void *p1, const void *p2)
{
	const luaL_Reg *one = (const luaL_Reg *) p1;
	const luaL_Reg *two = (const luaL_Reg *) p2;
	return strcmp(one->name, two->name);
}

void lux_qsort(luaL_Reg *regs, size_t size)
{
	qsort(regs, size, sizeof(luaL_Reg), compare);
}

lua_CFunction lux_bsearch(const char *name, const luaL_Reg *regs, size_t size)
{
	luaL_Reg key = {name, nullptr};
	union {
	 luaL_Reg *reg;
	 void *address;
	};
	reg = &key;
	address = bsearch(address, regs, size, sizeof(luaL_Reg), compare);
	return address ? reg->func : nullptr;
}


// LXSTACK.HPP ////////////////////////////////////////////////////////////////


int lux_stackdump(lua_State *state)
{
	int top = lua_gettop(state);
	for (int it = 1; it <= top; ++it)
	{
	 const char *name = luaL_typename(state, it);
	 switch (lua_type(state, it))
	 {
	   case LUA_TNIL:
	   case LUA_TNONE:
	   case LUA_TFUNCTION:
		printf("[%d] %s\n", it, name);
		break;
	   case LUA_TNUMBER:
		printf("[%d] %s: %f\n", it, name, lua_tonumber(state, it));
		break;
	   case LUA_TSTRING:
		printf("[%d] %s: %s\n", it, name, lua_tostring(state, it));
		break;
	   case LUA_TTHREAD:
		printf("[%d] %s: %p\n", it, name, lua_tothread(state, it));
		break;
	   case LUA_TBOOLEAN:
		printf("[%d] %s: %d\n", it, name, lua_toboolean(state, it));
		break;
	   case LUA_TLIGHTUSERDATA:
		printf("[%d] %s: %p\n", it, name, lua_touserdata(state, it));
		break;
	   case LUA_TTABLE:
	   case LUA_TUSERDATA:
		if (luaL_callmeta(state, it, "__tostring"))
		{
		 name = lua_tostring(state, -1);
		 printf("[%d] %s\n", it, name);
		 lua_pop(state, 1);
		}
		else
		 printf("[%d] %s: %p\n", it, name, lua_topointer(state, it));
		break;
	 }
	}
	return 0;
}

