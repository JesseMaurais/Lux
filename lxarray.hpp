#ifndef __lxarray__
#define __lxarray__

#include "lxtypes.hpp"
#include "lxstack.hpp"

// Emulate C arrays/pointers in Lua

template <class User> struct lux_Array
{
	// Pointer implementation
	typedef lux_Type<User*> Type;

	// Array allocation function
	static int __new(lua_State *state)
	{
		const char *string;
		size_t size;
		User *data;

		switch (lua_type(state, 1))
		{
		  case LUA_TNIL:
		  case LUA_TNONE:
			data = nullptr;
			size = 0; 
		  	break;
		  case LUA_TNUMBER:
			size = lux_to<int>(state, 1);
			data = new User [size];
			break;
		  case LUA_TSTRING:
			string = luaL_tolstring(state, 1, &size);
			data = new User [size+1];
			data[size] = '\0';
			for (int it = 0; it < size; ++it)
			{
			 data[it] = string[it];
			}
			break;
		  case LUA_TTABLE:
		  case LUA_TUSERDATA:
			size = luaL_len(state, 1);
			data = new User [size];
			lua_pushnil(state);
			for (int it = 0; lua_next(state, 1); ++it)
			{
			 data[it] = lux_to<User>(state, 3);
			 lua_pop(state, 1);
			}
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

	// Read array data at offset
	static int __index(lua_State *state)
	{
		User *data = lux_to<User*>(state, 1);
		int offset = lux_to<int>(state, 2);
		lux_push(state, data[offset]);
		return 1;
	}

	// Write array data at offset
	static int __newindex(lua_State *state)
	{
		User *data = lux_to<User*>(state, 1);
		int offset = lux_to<int>(state, 2);
		data[offset] = lux_to<User>(state, 3);
		return 0;
	}

	// Concatenate arrays of same type
	static int __concat(lua_State *state)
	{
		Type *one = Type::check(state, 1);
		Type *two = Type::check(state, 2);
		// If arrays but not pointers
		if (one->size && two->size)
		{
			// Calculate/create required storage
			size_t size = one->size + two->size;
			auto *data = new User [size];
			// Push userdata onto stack
			new (state) Type(data, size);
			luaL_setmetatable(state, Type::name);
			// Copy contents of the input arrays to new array
			for (int item = 0; item < one->size; ++item, ++data)
				*data = one->data[item];
			for (int item = 0; item < two->size; ++item, ++data)
				*data = two->data[item];
			return 1;
		}
		return 0;
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

	// Size of array or zero if pointer
	static int __len(lua_State *state)
	{
		Type *user = Type::check(state);
		lux_push(state, user->size);
		return 1;
	}

	// Loader compatible with luaL_requiref
	static int open(lua_State *state)
	{
		Type::name = luaL_checkstring(state, 1);
		luaL_newmetatable(state, Type::name);
		luaL_Reg regs [] =
		{
		{"new", __new},
		{"__tostring", __tostring},
		{"__concat", __concat},
		{"__newindex", __newindex},
		{"__index", __index},
		{"__add", __add},
		{"__sub", __sub},
		{"__len", __len},
		{"__gc", __gc},
		{nullptr}
		};
		luaL_setfuncs(state, regs, 0);
		return 1;
	}
};

#endif // file

