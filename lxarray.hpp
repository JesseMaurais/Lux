#ifndef __lxarray__
#define __lxarray__

#include "lxalloc.hpp"
#include "lxstack.hpp"

// register POD array types
int lux_openarray(lua_State *state);


template <class C> struct lux_Array
{
	typedef lux_Type<C*> Type;

	static int __new(lua_State *state)
	{
		const char *string;
		size_t size;
		C *data;

		switch (lua_type(state, 1))
		{
		  case LUA_TNIL:
		  case LUA_TNONE:
			data = nullptr;
			size = 0; 
		  	break;
		  case LUA_TNUMBER:
			size = lua_tointeger(state, 1);
			data = new C [size];
			break;
		  case LUA_TSTRING:
			string = luaL_tolstring(state, 1, &size);
			data = new C [size+1];
			data[size] = '\0';
			for (int it = 0; it < size; ++it)
			{
			 data[it] = string[it];
			}
			break;
		  case LUA_TTABLE:
			size = luaL_len(state, 1);
			data = new C [size];
			lua_pushnil(state);
			for (int it = 0; lua_next(state, 1); ++it)
			{
			 data[it] = lux_to<C>(state, 3);
			 lua_pop(state, 1);
			}
			break;
		  default:
			return luaL_argerror(state, 1, "invalid type");
		};

		lux_push(state, data, size);
		return 1;
	}

	static int __gc(lua_State *state)
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

	static int __index(lua_State *state)
	{
		auto data = lux_to<C*>(state, 1);
		int offset = luaL_checkinteger(state, 2);
		lux_push<C>(state, data[offset]);
		return 1;
	}

	static int __newindex(lua_State *state)
	{
		auto data = lux_to<C*>(state, 1);
		int offset = luaL_checkinteger(state, 2);
		data[offset] = lux_to<C>(state, 3);
		return 0;
	}

	static int __add(lua_State *state)
	{
		auto data = lux_to<C*>(state, 1);
		int offset = luaL_checkinteger(state, 2);
		lux_push<C*>(state, data + offset);
		return 1;
	}

	static int __sub(lua_State *state)
	{
		auto data = lux_to<C*>(state, 1);
		int offset = luaL_checkinteger(state, 2);
		lux_push<C*>(state, data - offset);
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

		lua_pushliteral(state, "__tostring");
		lua_pushcfunction(state, __tostring);
		lua_settable(state, -3);

		lua_pushliteral(state, "__newindex");
		lua_pushcfunction(state, __newindex);
		lua_settable(state, -3);

		lua_pushliteral(state, "__index");
		lua_pushcfunction(state, __index);
		lua_settable(state, -3);

		lua_pushliteral(state, "__add");
		lua_pushcfunction(state, __add);
		lua_settable(state, -3);

		lua_pushliteral(state, "__sub");
		lua_pushcfunction(state, __sub);
		lua_settable(state, -3);

		lua_pushliteral(state, "__len");
		lua_pushcfunction(state, __len);
		lua_settable(state, -3);

		lua_pushliteral(state, "__gc");
		lua_pushcfunction(state, __gc);
		lua_settable(state, -3);
		
		lua_register(state, Type::name, __new);
		lua_pop(state, 1);
		return 0;
	}

	static void require(lua_State *state, bool global=true)
	{
		luaL_requiref(state, Type::name, open, global);
	}
};

#endif // file

