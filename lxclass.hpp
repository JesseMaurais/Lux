#ifndef __lxclass__
#define __lxclass__

#include "lxalloc.hpp"
#include "lxstack.hpp"

template <class C> struct lux_Class
{
	typedef lux_Type<C*> Type;
	typedef lux_User<C*> User;

	static int __new(lua_State  *state)
	{
		size_t size = 1;
		C *data = nullptr;

		switch (lua_type(state, 1))
		{
		  case LUA_TNIL:
			size = 0;
			break;
		  case LUA_TNUMBER:
			size = lua_tointeger(state, 1);
		  case LUA_TNONE:
			data = new C [size];	
			break;
		  default:
			return luaL_argerror(state, 1, "number, none, nil");
		};

		lux_push(state, data, size);
		return 1;
	}

	static int __delete(lua_State *state)
	{
		auto user = lux_check<C*>(state, 1);
		if (user->size && user->data)
		{
		 delete [] user->data;
		}
		return 0;
	}

	static int __tostring(lua_State *state)
	{
		void *address = luaL_checkudata(state, 1, Type::name);
		lua_pushfstring(state, "%s: %p", Type::name, address);
		return 1;
	}

	static int __add(lua_State *state)
	{
		auto ptr = lux_to<C*>(state, 1);
		ptrdiff_t diff = lua_tointeger(state, 2);
		lux_push(state, ptr + diff);
		return 1;
	}

	static int __len(lua_State *state)
	{
		auto user = lux_check<C*>(state, 1);
		lua_pushinteger(state, user->size);
		return 1;
	}

	static int open(lua_State *state)
	{
		luaL_newmetatable(state, Type::name);
		int meta = lua_gettop(state);
		lua_newtable(state);
		int tab = lua_gettop(state);

		lua_pushliteral(state, "__metatable");
		lua_pushvalue(state, tab);
		lua_settable(state, meta);

		lua_pushliteral(state, "__index");
		lua_pushvalue(state, tab);
		lua_settable(state, meta);

		lua_pushliteral(state, "__tostring");
		lua_pushcfunction(state, __tostring);
		lua_settable(state, meta);

		lua_pushliteral(state, "__add");
		lua_pushcfunction(state, __add);
		lua_settable(state, meta);

		lua_pushliteral(state, "__len");
		lua_pushcfunction(state, __len);
		lua_settable(state, meta);

		lua_pushliteral(state, "__gc");
		lua_pushcfunction(state, __delete);
		lua_settable(state, meta);

		setfuncs(state);
		lua_register(state, Type::name, __new);
		lua_pop(state, 2);
		return 0;
	}

	static void setfuncs(lua_State *state);
};


#endif // file

