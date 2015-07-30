#ifndef __lxclass__
#define __lxclass__

#include "lxalloc.hpp"
#include "lxstore.hpp"
#include "lxstack.hpp"

// Export C++ class/struct/union to Lua

template <class User> struct lux_Class
{
	// Pointer storage implementation
	typedef lux_Store<User*> Type;

	// Array allocation function
	static int __new(lua_State *state)
	{
		size_t size = 1;
		User *data = nullptr;
		switch (lua_type(state, 1))
		{
		  case LUA_TNIL:
			// nullptr
			size = 0;
			break;
		  case LUA_TNUMBER:
			// array
			size = lux_to<int>(state, 1);
		  case LUA_TNONE:
			// single
			if (size) data = new User [size];
			break;
		  case LUA_TUSERDATA:
			// cast
			data = (User*) lux_to<void*>(state, 1);
			size = 0;
			break;
		  default:
			return luaL_argerror(state, 1, "invalid type");
		};
		Type::push(state, data, size);
		luaL_setmetatable(state, Type::name);
		return 1;
	}

	// Garbage collection callback
	static int __gc(lua_State *state)
	{
		Type *user = Type::check(state);
		if (user->size) delete [] user->data;
		return 0;
	}

	// String conversion for printing
	static int __tostring(lua_State *state)
	{
		lua_pushfstring(state, "%s: %p", Type::name, Type::to(state));
		return 1;
	}

	// Size of array or zero if pointer
	static int __len(lua_State *state)
	{
		Type *user = Type::check(state);
		lux_push(state, user->size);
		return 1;
	}

	// Pointer addition arithmetic
	static int __add(lua_State *state)
	{
		User *data = lux_to<User*>(state, 1);
		int offset = lux_to<int>(state, 2);
		lux_push(state, data + offset);
		return 1;
	}

	// Pointer subtraction arithmetic
	static int __sub(lua_State *state)
	{
		User *data = lux_to<User*>(state, 1);
		int offset = lux_to<int>(state, 2);
		lux_push(state, data - offset);
		return 1;
	}

	// Read/Write into given fields of the record
	template <class Base>
	static int member(lua_State *state, Base User::*field)
	{
		User *data = lux_to<User*>(state, 1);
		// Write if argument given
		if (lua_gettop(state) > 1)
		{
		 data->*field = lux_to<Base>(state, 2);
		 return 0;
		}
		else
		{
		 lux_push<Base>(state, data->*field);
		 return 1;
		}
	}

	// Loader compatible with luaL_requiref
	static int open(lua_State *state)
	{
		Type::name = lua_tostring(state, +1);
		luaL_newmetatable(state, Type::name);

		lua_pushliteral(state, "new");
		lua_pushcfunction(state, __new);
		lua_settable(state, -3);

		lua_pushliteral(state, "__gc");
		lua_pushcfunction(state, __gc);
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

		lua_pushliteral(state, "__tostring");
		lua_pushcfunction(state, __tostring);
		lua_settable(state, -3);

		if (index->name)
		{
		lua_pushliteral(state, "__index");
		luaL_newlib(state, index);
		lua_settable(state, -3);
		}

		return 1;
	}

	static luaL_Reg index[];
};

// Assume empty unless user specifies otherwise

template <class User> luaL_Reg lux_Class<User>::index [] = {{nullptr}};

// Wrap a member accessor in an anonymous function compatible with Lua

#define lux_member(C, x) \
	[](lua_State *state)->int { return lux_Class<C>::member(state, &C::x); }

// Use a similar syntax for class methods

#define lux_method(C, x) lux_cast(C::x)


#endif // file

