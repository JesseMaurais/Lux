#ifndef __lxunion__
#define __lxunion__

#include "lxalloc.hpp"
#include "lxstack.hpp"
#include <stdexcept>
#include <string>
#include <map>

template <class C> struct lux_Union
{
	typedef lux_Type<C*> Type;

	static int __new(lua_State *state)
	{
		size_t size = 1;
		C *data = nullptr;

		switch (lua_type(state, 1))
		{
		  case LUA_TNIL:
			lux_push(state, data, 0);
			break;
		  case LUA_TNONE:
			lux_push(state, new C, size);
			break;
		  case LUA_TNUMBER:
			size = lua_tointeger(state, 1);
			if (size) data = new C [size];
			lux_push(state, data, size);
			break;
		  case LUA_TTABLE:
			lux_push(state, new C, size);
			lua_pushnil(state);
			while (lua_next(state, 1))
			{
			 __newindex(state);
			 lua_pop(state, 1);
			}
			break;
		  default:
			return luaL_argerror(state, 1, "number, table, none");
		};

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

	static int __newindex(lua_State *state)
	{
		const char *key = luaL_checkstring(state, -2);
		try {
		 return newindex.at(key)(state);
		}
		catch (std::out_of_range) {
		 return luaL_error(state, "%s.%s = ...", Type::name, key);
		}
	}

	static int __index(lua_State *state)
	{
		const char *key = luaL_checkstring(state, -1);
		try {
		 return index.at(key)(state);
		}
		catch (std::out_of_range) {
		 return luaL_error(state, "... = %s.%s", Type::name, key);
		}
	}

	static int __len(lua_State *state)
	{
		auto user = lux_check<C*>(state, 1);
		lua_pushinteger(state, user->size);
		return 1;
	}

	static int __add(lua_State *state)
	{
		auto address = lux_to<C*>(state, 1);
		ptrdiff_t offset = luaL_checkint(state, 2);
		lux_push(state, address + offset);
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

		lua_pushliteral(state, "__len");
		lua_pushcfunction(state, __len);
		lua_settable(state, -3);

		lua_pushliteral(state, "__gc");
		lua_pushcfunction(state, __delete);
		lua_settable(state, -3);

		lua_register(state, Type::name, __new);
		lua_pop(state, 1);
		setfuncs(state);
		return 0;
	}

	template <class X, int offset> static X *at(lua_State *state, int index)
	{
		union {
		 C *buffer;
		 char *address;
		 X *object;
		};
		buffer = lux_to<C*>(state, index);
		address += offset;
		return object;
	}

	template <class X, int offset> static int set(lua_State *state)
	{
		*at<X, offset>(state, -3) = lux_to<X>(state, -1);
		return 0;
	}

	template <class X, int offset> static int get(lua_State *state)
	{
		lux_push<X>(state, *at<X, offset>(state, -2));
		return 1;
	}

	template <class X, int offset> static void set(const char *key)
	{
		newindex[key] = set<X, offset>;
	}

	template <class X, int offset> static void get(const char *key)
	{
		index[key] = get<X, offset>;
	}

	static std::map<std::string, lua_CFunction> index;
	static std::map<std::string, lua_CFunction> newindex;

	static int setfuncs(lua_State *state);
};


template <class C> std::map<std::string, lua_CFunction> lux_Union<C>::index;
template <class C> std::map<std::string, lua_CFunction> lux_Union<C>::newindex;

#define lux_set(C, x) lux_Union<C>::set<decltype(C::x), offsetof(C, x)>(#x)
#define lux_get(C, x) lux_Union<C>::get<decltype(C::x), offsetof(C, x)>(#x)


#endif // file

