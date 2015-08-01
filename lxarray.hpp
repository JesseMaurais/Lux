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
		// Constructor arguments
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
			// Copy table contents
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
			// Get UTF-8 encoded string with length
			string = lua_tolstring(state, 1, &size);
			length = shift.stringsize(string, size);
			// Check the string for encoding errors
			if (length < 0)	return lux_argerror(state, 1);
			else shift.reset();
			// Create and convert
			data = new User [length];
			size = shift.to(data, string, length);
			break;
		  default:
			return luaL_argerror(state, 1, "invalid type");
		};
		// Put the array on the stack
		Type::push(state, data, size);
		luaL_setmetatable(state, Type::name);
		return 1;
	}

	// Garbage collection callback
	static int __gc(lua_State *state)
	{
		Type *user = Type::check(state);
		// Owner's have non-negative sizes
		if (0 < user->size) delete [] user->data;
		return 0;
	}

	// Size of array or zero if pointer
	static int __len(lua_State *state)
	{
		Type *user = Type::check(state);
		lux_push(state, abs(user->size));
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

	// String conversion for printing
	static int __tostring(lua_State *state)
	{
		Type *user = Type::check(state);
		size_t size = abs(user->size);
		// Build up a string
		lux_Buffer buffer(state);
		// Iterate over each item in the array
		for (auto item = 0; item < size; ++item)
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

	// Concatenate arrays of same type
	static int __concat(lua_State *state)
	{
		Type *one = Type::check(state, 1);
		Type *two = Type::check(state, 2);
		// Genuine size of arrays
		size_t n = abs(one->size);
		size_t m = abs(two->size);
		// Pointers are valid?
		if (n > 0 and m > 0)
		{
			// Required storage
			size_t size = n + m;
			User *data = new User [size];
			// Push userdata onto stack
			new (state) Type(data, size);
			luaL_setmetatable(state, Type::name);
			// Copy contents of input arrays
			memcpy(data + n, two->data, m * sizeof(User));
			memcpy(data, one->data, n * sizeof(User));
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

	// Shift values left by given amount
	static int __shl(lua_State *state)
	{
		Type *user = Type::check(state, 1);
		ssize_t shift = luaL_checkinteger(state, 2);
		// We only accept positive values for shifting
		luaL_argcheck(state, (shift > 0), 2, "shift > 0");
		// Shorthand variables
		size_t t = user->size;
		size_t s = shift;
		size_t n = s % t;
		size_t m = t - n;
		// Copy without overlap
		if (n < m)
		{
		User temp[m];
		memcpy(temp, user->data + n, m * sizeof(User));
		memcpy(user->data + m, user->data, n * sizeof(User));
		memcpy(user->data, temp, m * sizeof(User));
		}
		else
		{
		User temp[n];
		memcpy(temp, user->data, n * sizeof(User));
		memcpy(user->data, user->data + n, m * sizeof(User));
		memcpy(user->data + m, temp, n * sizeof(User));
		}
		lua_pushvalue(state, 1);
		return 1;
	}

	// Shift values right by given amount
	static int __shr(lua_State *state)
	{
		Type *user = Type::check(state, 1);
		ssize_t shift = luaL_checkinteger(state, 2);
		// We only accept positive values for shifting
		luaL_argcheck(state, (shift > 0), 2, "shift > 0");
		// Shorthand variables
		size_t t = user->size;
		size_t s = shift;
		size_t n = s % t;
		size_t m = t - n;
		// Copy without overlap
		if (n < m)
		{
		User temp[m];
		memcpy(temp, user->data, m * sizeof(User));
		memcpy(user->data, user->data + m, n * sizeof(User));
		memcpy(user->data + n, temp, m * sizeof(User));
		}
		else
		{
		User temp[n];
		memcpy(temp, user->data + m, n * sizeof(User));
		memcpy(user->data + m, user->data, m * sizeof(User));
		memcpy(user->data, temp, n * sizeof(User));
		}
		lua_pushvalue(state, 1);
		return 1;
	}

	// Element comparison operator used for qsort
	static int compare(const void *p1, const void *p2)
	{
		User e1 = *(const User *) p1;
		User e2 = *(const User *) p2;
		return e1 < e2 ? -1 : e1 > e2 ? 1 : 0;
	}

	// Quick sort of the array elements
	static int __qsort(lua_State *state)
	{
		Type *user = Type::check(state);
		qsort(user->data, abs(user->size), sizeof(User), compare);
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
		{"qsort", __qsort},
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
		{"__shl", __shl},
		{"__shr", __shr},
		{"__len", __len},
		{"__gc", __gc},
		{nullptr}
		};
		luaL_setfuncs(state, regs, 0);
		return 1;
	}
};

#endif // file

