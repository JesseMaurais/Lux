#ifndef __lxarray__
#define __lxarray__

#include "lxalloc.hpp"
#include "lxstore.hpp"
#include "lxstack.hpp"
#include "lxbuffs.hpp"
#include "lxchars.hpp"
#include "lxerror.hpp"

// Emulate C arrays/pointers in Lua

template <class User> struct lux_Array
{
	// Pointer storage implementation
	typedef lux_Store<User*> Type;

	// Array allocation function
	static int __new(lua_State *state)
	{
		lux_Chars shift;
		const char *string;
		ssize_t length;
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
		  case LUA_TSTRING:
			string = lua_tolstring(state, 1, &size);
			length = shift.stringsize(string, size);
			if (length < 0)
			return luaL_argerror(state, 1, "cannot convert");
			else shift.reset();
			data = new User [length];
			size = shift.to(data, string, length);
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
		Type *user = Type::check(state);
		// Build up a string
		lux_Buffer buffer(state);
		// Iterate over each item in the array
		for (auto item = 0; item < user->size; ++item)
		{
			// Add separator only after first
			if (item) buffer.addstring(", ");
			// Convert each element to string
			lux_push(state, user->data[item]);
			buffer.addvalue();
		}
		buffer.push();
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

	// Read as string from a file
	static int __fgets(lua_State *state)
	{
		Type *user = Type::check(state, 1);
		FILE *file = lux_to<FILE*>(state, 2);
		// Buffer to store UTF-8 characters
		char data[user->size * MB_CUR_MAX];
		// Read from the file stream
		fgets(data, sizeof(data), file);
		// Conversion
		lux_Chars shift;
		ssize_t size = shift.to(user->data, data, sizeof(data));
		if (size < 0) return lux_perror(state);
		// Return bytes read
		lux_push(state, size);
		return 1;
	}

	// Write as string to a file
	static int __fputs(lua_State *state)
	{
		Type *user = Type::check(state, 1);
		FILE *file = lux_to<FILE*>(state, 2);
		// Buffer to store UTF-8 characters
		char data[user->size * MB_CUR_MAX];
		// Conversion
		lux_Chars shift;
		ssize_t size = shift.from(data, user->data, user->size);
		if (size < 0) return lux_perror(state);
		// Write to the file stream
		data[size] = '\0';
		fputs(data, file);
		return 0;
	}

	// Read binary elements from a file
	static int __fread(lua_State *state)
	{
		Type *user = Type::check(state, 1);
		FILE *file = lux_to<FILE*>(state, 2);
		size_t num = fread(user->data, user->size, sizeof(User), file);
		lux_push(state, num);
		return 1;
	}

	// Write binary elements to a file
	static int __fwrite(lua_State *state)
	{
		Type *user = Type::check(state, 1);
		FILE *file = lux_to<FILE*>(state, 2);
		size_t num = fwrite(user->data, user->size, sizeof(User), file);
		lux_push(state, num);
		return 1;
	}

	// Loader compatible with luaL_requiref
	static int open(lua_State *state)
	{
		Type::name = lua_tostring(state, +1);
		luaL_newmetatable(state, Type::name);
		luaL_Reg regs [] =
		{
		{"new", __new},
		{"fgets", __fgets},
		{"fputs", __fputs},
		{"fread", __fread},
		{"fwrite", __fwrite},
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

