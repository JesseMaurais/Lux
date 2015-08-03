#ifndef __lxarray__
#define __lxarray__

#include "lxalloc.hpp"
#include "lxstore.hpp"
#include "lxstack.hpp"
#include "lxbuffs.hpp"
#include "lxchars.hpp"
#include "lxerror.hpp"

// Emulate C strings/arrays/pointers in Lua with extra operations

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
			size = lua_tointeger(state, 1);
			lux_checkarg(state, 1, 0 < size);
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
		return 1;
	}

	// Garbage collection callback
	static int __gc(lua_State *state)
	{
		Type *user = Type::check(state);
		// Owner's have non-negative sizes
		if (0 < user->size) delete [] user->data;
		else
		{
		luaL_getmetatable(state, Type::name);
		// Free reference to the data owner
		luaL_unref(state, -1, user->ref);
		}
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
		Type *user = Type::check(state, 1);
		int offset = luaL_checkinteger(state, 2);
		// Pointer indexing forbidden
		size_t size = abs(user->size);
		lux_checkarg(state, 1, 0 < size);
		// Ensure we stay in array bounds
		lux_checkarg(state, 2, 0 < offset);
		lux_checkarg(state, 2, offset <= size);
		// C arrays are indexed from 0 rather than 1
		lux_push(state, user->data[offset-1]);
		return 1;
	}

	// Write array data at offset
	static int __newindex(lua_State *state)
	{
		Type *user = Type::check(state, 1);
		int offset = luaL_checkinteger(state, 2);
		// Pointer indexing forbidden
		size_t size = abs(user->size);
		lux_checkarg(state, 1, 0 < size);
		// Ensure we stay in array bounds
		lux_checkarg(state, 2, 0 < offset);
		lux_checkarg(state, 2, offset <= size);
		// C arrays are indexed from 0 rather than 1
		user->data[offset-1] = lux_to<User>(state, 3);
		return 1;
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
			buffer.addvalue(); // Pops string
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
			// Copy contents of input arrays
			memcpy(data, one->data, n * sizeof(User));
			memcpy(data + n, two->data, m * sizeof(User));
			// Put array onto the stack
			Type::push(state, data, size);
			return 1;
		}
		return 0;
	}

	// Pointer addition arithmetic
	static int __add(lua_State *state)
	{
		Type *user = Type::check(state, 1);
		int offset = luaL_checkinteger(state, 2);
		// Force offset to be positive
		lux_checkarg(state, 2, 0 <= offset);
		// Adjust the pointer address
		User *data = user->data + offset;
		// Adjust the (phony) array size
		ssize_t size = abs(user->size);
		if (0 < size) // Non-pointer
		{
			// Ensure that we stay in array range
			lux_checkarg(state, 2, offset < size);
			// Shrink range
			size -= offset;
			// Not owner
			size = -size;
		}
		// Put referenced array on stack
		Type::push(state, data, size, 1);
		return 1;
	}

	// Pointer subtraction arithmetic
	static int __sub(lua_State *state)
	{
		Type *user = Type::check(state, 1);
		int offset = luaL_checkinteger(state, 2);
		// Force offset to be positive
		lux_checkarg(state, 2, 0 <= offset);
		// Adjust the (phony) array size
		ssize_t size = abs(user->size);
		if (0 < size) // Non-pointer
		{
			// Ensure that we stay in range
			lux_checkarg(state, 2, offset < size);
			// Shrink range
			size -= offset;
			// Not owner
			size = -size;
			// Put referenced array on the stack
			Type::push(state, user->data, size, 1);
		}
		else // Pointer
		{
			// Adjust the pointer address
			User *data = user->data - offset;
			// Put pointer on the stack
			Type::push(state, data);
		}
		return 1;
	}

	// Shift elements left by given amount
	static int __shl(lua_State *state)
	{
		Type *user = Type::check(state, 1);
		ssize_t shift = luaL_checkinteger(state, 2);
		// We only accept positive values for shifting
		lux_checkarg(state, 2, 0 < shift);
		size_t size = abs(user->size);
		lux_checkarg(state, 1, 0 < size);
		// Proportions used in shift
		size_t n = shift % size;
		size_t m = size - n;
		// Swap memory
		if (n < m)
		{
			User temp[m]; // Copy to avoid overwrite
			memcpy(temp, user->data + n, m*sizeof(User));
			memcpy(user->data + m, user->data, n*sizeof(User));
			memcpy(user->data, temp, m*sizeof(User));
		}
		else
		{
			User temp[n]; // Copy to avoid overwrite
			memcpy(temp, user->data, n*sizeof(User));
			memcpy(user->data, user->data + n, m*sizeof(User));
			memcpy(user->data + m, temp, n*sizeof(User));
		}
		// Return a reference
		lua_pushvalue(state, 1);
		return 1;
	}

	// Shift element right by given amount
	static int __shr(lua_State *state)
	{
		Type *user = Type::check(state, 1);
		ssize_t shift = luaL_checkinteger(state, 2);
		// We only accept positive values for shifting
		lux_checkarg(state, 2, 0 < shift);
		size_t size = abs(user->size);
		lux_checkarg(state, 1, 0 < size);
		// Proportions used in shift
		size_t n = shift % size;
		size_t m = size - n;
		// Swap memory
		if (m < n)
		{
			User temp[n]; // Copy to avoid overwrite
			memcpy(temp, user->data + m, n*sizeof(User));
			memcpy(user->data + n, user->data, m*sizeof(User));
			memcpy(user->data, temp, n*sizeof(User));
		}
		else
		{
			User temp[m]; // Copy to avoid overwrite
			memcpy(temp, user->data, m*sizeof(User));
			memcpy(user->data, user->data + m, n*sizeof(User));
			memcpy(user->data + n, temp, m*sizeof(User));
		}
		// Return a reference
		lua_pushvalue(state, 1);
		return 1;
	}

	// Reverse elements in the array
	static int __bnot(lua_State *state)
	{
		Type *user = Type::check(state);
		size_t size = abs(user->size);
		// Array rather than a pointer
		lux_checkarg(state, 1, 0 < size);
		// Half size of the array
		size_t half = size >> 1;
		User temp; // Swap elements
		for (int i = 0, j = size - 1; i < half; ++i, --j)
		{
			temp = user->data[i];
			user->data[i] = user->data[j];
			user->data[j] = temp;
		}
		// Return a reference
		lua_pushvalue(state, 1);
		return 1;
	}

	// Quick sort of the array elements
	static int __sort(lua_State *state)
	{
		// Element comparison operator used for quick sort
		auto compare = [](const void *p1, const void *p2)->int
		{
			// Cast to the userdata type
			User a = *(const User *) p1;
			User b = *(const User *) p2;
			// Compare as numeric values
			return a < b ? -1 : a > b ? +1 : 0;
		};
		Type *user = Type::check(state);
		qsort(user->data, abs(user->size), sizeof(User), compare);
	}

	// Check equality of elements
	static int __eq(lua_State *state)
	{
		Type *one = Type::check(state, 1);
		Type *two = Type::check(state, 2);
		// Not equal if sizes aren't
		if (one->size != two->size)
		{
			lux_push(state, false);
			return 1;
		}
		// Are equal if pointers are
		if (one->data == two->data)
		{
			lux_push(state, true);
			return 1;
		}
		size_t size = abs(one->size);
		// Equal if, and only if, all the bits are identical
		if (memcmp(one->data, two->data, size * sizeof(User)))
		{
			lux_push(state, false);
			return 1;
		}
		// Then one == two
		lux_push(state, true);
		return 1;
	}

	// Lexical less than comparison
	static int __lt(lua_State *state)
	{
		Type *one = Type::check(state, 1);
		Type *two = Type::check(state, 2);
		// Genuine array sizes
		size_t n = abs(one->size);
		size_t m = abs(two->size);
		// Find the smaller of them
		size_t size = n < m ? n : m;
		// Compare each respective element
		for (int item = 0; item < size; ++item)
		{
			// Return on first negative result
			if (one->data[item] >= two->data[item])
			{
				lux_push(state, false);
				return 1;
			}
		}
		// Then one < two
		lux_push(state, true);
		return 1;
	}

	// Lexical less than or equal
	static int __le(lua_State *state)
	{
		Type *one = Type::check(state, 1);
		Type *two = Type::check(state, 2);
		// Genuine array sizes
		size_t n = abs(one->size);
		size_t m = abs(two->size);
		// Find the smaller of them
		size_t size = n < m ? n : m;
		// Compare each respective element
		for (int item = 0; item < size; ++item)
		{
			// Return on first negative result
			if (one->data[item] > two->data[item])
			{
				lux_push(state, false);
				return 1;
			}
		}
		// Then one <= two
		lux_push(state, true);
		return 1;
	}

	// Read as string from a file
	static int __gets(lua_State *state)
	{
		Type *user = Type::check(state, 1);
		FILE *file = lux_opt(state, 2, stdin);
		// Buffer to store UTF-8 characters
		ssize_t size = abs(user->size);
		char data[size * MB_CUR_MAX];
		// Using fgets on given file
		fgets(data, sizeof(data), file);
		// Conversion
		lux_Chars shift;
		size = shift.to(user->data, data, sizeof(data));
		if (size < 0) return lux_perror(state);
		// Return amount read
		lux_push(state, size);
		return 1;
	}

	// Write as string to a file
	static int __puts(lua_State *state)
	{
		Type *user = Type::check(state, 1);
		FILE *file = lux_opt(state, 2, stdout);
		// Buffer to store UTF-8 characters
		ssize_t size = abs(user->size);
		char data[size * MB_CUR_MAX];
		// Conversion
		lux_Chars shift;
		size = shift.from(data, user->data, size);
		if (size < 0) return lux_perror(state);
		// Using fputs on given file
		data[size] = '\0';
		fputs(data, file);
		return 0;
	}

	// Read binary elements from a file
	static int __read(lua_State *state)
	{
		Type *user = Type::check(state, 1);
		FILE *file = lux_opt(state, 2, stdin);
		// Using fread on given file
		size_t size = abs(user->size);
		size = fread(user->data, size, sizeof(User), file);
		lux_push(state, size);
		return 1;
	}

	// Write binary elements to a file
	static int __write(lua_State *state)
	{
		Type *user = Type::check(state, 1);
		FILE *file = lux_opt(state, 2, stdout);
		// Using fwrite on given file
		size_t size = abs(user->size);
		size = fwrite(user->data, size, sizeof(User), file);
		lux_push(state, size);
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
		{"gets", __gets},
		{"puts", __puts},
		{"read", __read},
		{"write", __write},
		{"sort", __sort},
		{"__tostring", __tostring},
		{"__concat", __concat},
		{"__newindex", __newindex},
		{"__index", __index},
		{"__len", __len},
		{"__add", __add},
		{"__sub", __sub},
		{"__shl", __shl},
		{"__shr", __shr},
		{"__bnot", __bnot},
		{"__eq", __eq},
		{"__lt", __lt},
		{"__le", __le},
		{"__gc", __gc},
		{nullptr}
		};
		luaL_setfuncs(state, regs, 0);
		return 1;
	}
};

#endif // file

