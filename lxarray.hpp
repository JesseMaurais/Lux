#ifndef __lxarray__
#define __lxarray__

/**
 * This is the preferred implementation of C arrays and pointers for Lua. The
 * array is guaranteed to be a contiguous area of memory to which writing and
 * reading elements is done with the index metamethods. Several operations to
 * arrays are also provided through other metamethods like parting, rotating,
 * and reversing elements. Creating sub-arrays which point to some contiguous
 * area within an array is also possible, where changes in one array are also
 * reflected automatically in the other. Memory is properly reference counted
 * in this case to avoid segfaults. Raw pointers are implemented in this same
 * metatable but the array functions are not. Pointers are supported only for
 * system related functions. Finally, arrays can store character data just as
 * C does it, only that arrays implicitly support multibyte decoding assuming
 * the input is in the Unicode transformation format. Lua strings are decoded
 * this way to the bit width of the array type. 
 *
 * See lxstore.hpp for the storage classes and lxchars.hpp for char decoding.
 * See array.cpp for the C++ source code that compiles into the array module. 
 */

#include "lxalloc.hpp"
#include "lxstore.hpp"
#include "lxstack.hpp"
#include "lxbuffs.hpp"
#include "lxchars.hpp"
#include "lxerror.hpp"

/// Emulate C strings/arrays/pointers in Lua with extra operations

template <class User> struct lux_Array
{
	// Pointer storage implementation
	typedef lux_Store<User*> Type;

	// Construct this array from an integer
	static int fromnumber(lua_State *state)
	{
		// Argument is the array's size
		int size = lua_tointeger(state, 1);
		// Check that the size is valid
		lux_argcheck(state, 1, 0 < size);
		// Create the data for array
		User *data = new User [size];
		// Put the array on the stack
		Type::push(state, data, size);
		return 1;
	}

	// Construct this array from a table
	static int fromtable(lua_State *state)
	{
		int size = luaL_len(state, 1);
		User *data = new User [size];
		lua_pushnil(state); // first
		// Copy every element of the table as numeric
		for (int item = 0; lua_next(state, 1); ++item)
		{
			// Convert the element to User type
			data[item] = lux_to<User>(state, 3);
			lua_pop(state, 1); // pop key
		}
		// Put the array on the stack
		Type::push(state, data, size);
		return 1;
	}

	// Construct this array from a string
	static int fromstring(lua_State *state)
	{
		size_t size;
		lux_Chars shift;
		// Get UTF-8 encoded string and it's size in bytes
		const char *string = lua_tolstring(state, 1, &size);
		// Find the number multibyte characters
		int length = shift.stringsize(string, size);
		// Check the string for encoding errors
		if (length < 0) return lux_argerror(state, 1);
		else shift.reset();
		// Create array and convert
		User *data = new User [length];
		size = shift.to(data, string, length);
		// Put the array on the stack
		Type::push(state, data, size);
		return 1;
	}

	/// Array allocation function
	static int __new(lua_State *state)
	{
		switch (lua_type(state, 1))
		{
		case LUA_TNUMBER:
			// Given the array size
			return fromnumber(state);
		case LUA_TTABLE:
			// Copy table contents
			return fromtable(state);
		case LUA_TSTRING:
			// Convert UTF-8 string
			return fromstring(state);
		};
		// We only construct from these argument types
		return luaL_argerror(state, 1, "number, table, string");
	}

	/// Garbage collection callback
	static int __gc(lua_State *state)
	{
		Type *user = Type::check(state);
		// Only owner has non-negative size
		if (0 < user->size) delete [] user->data;
		else
		{
		luaL_getmetatable(state, Type::name);
		// Free reference to the data owner
		luaL_unref(state, -1, user->ref);
		}
		return 0;
	}

	/// Size of array or zero if pointer
	static int __len(lua_State *state)
	{
		Type *user = Type::check(state);
		lux_push(state, abs(user->size));
		return 1;
	}

	/// Read array data at offset
	static int __index(lua_State *state)
	{
		Type *user = Type::check(state, 1);
		// Pointer indexing forbidden
		int size = abs(user->size);
		lux_argcheck(state, 1, 0 < size);
		// Ensure we stay in array bounds
		int index = luaL_checkinteger(state, 2);
		if (0 < index and index <= size)
		{
		// Arrays are indexed from 1, not 0
		lux_push(state, user->data[--index]);
		}
		else
		{
		// By convention
		lua_pushnil(state);
		}
		return 1;
	}

	/// Write array data at offset
	static int __newindex(lua_State *state)
	{
		Type *user = Type::check(state, 1);
		// Pointer indexing forbidden
		int size = abs(user->size);
		lux_argcheck(state, 1, 0 < size);
		// Ensure we stay in array bounds
		int index = luaL_checkinteger(state, 2);
		if (0 < index and index <= size)
		{
		// Arrays are indexed from 1 rather than 0
		user->data[--index] = lux_to<User>(state, 3);
		}
		else // out of bounds
		{
		// Report error to the interpreter so it can do a stack trace
		luaL_error(state, "assignment to %d is out of bounds", index);
		}
		return 0;
	}

	/// String conversion for printing
	static int __tostring(lua_State *state)
	{
		Type *user = Type::check(state);
		int size = abs(user->size);
		// Build up a string
		lux_Buffer buffer(state);
		// Iterate over each item in the array
		for (int item = 0; item < size; ++item)
		{
			// Add separator only after first
			if (item) buffer.addstring(", ");
			// Convert each element to string
			lux_push(state, user->data[item]);
			if (luaL_callmeta(state, -1, "__tostring"))
			{
				buffer.addvalue(); // add string
				lua_pop(state, 1); // pop value
				continue;
			}
			// Try the value
			buffer.addvalue();
		}
		buffer.push();
		return 1;
	}

	/// Concatenate arrays of same type
	static int __concat(lua_State *state)
	{
		Type *one = Type::check(state, 1);
		Type *two = Type::check(state, 2);
		// Genuine array sizes
		int n = abs(one->size);
		int m = abs(two->size);
		// Neither pointers
		if (n > 0 and m > 0)
		{
			int size = n + m;
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

	/// Partition into equal sub-arrays
	static int __mul(lua_State *state)
	{
		Type *user = Type::check(state, 1);
		// Pointer division forbidden
		int size = abs(user->size);
		lux_argcheck(state, 1, 0 < size);
		// Force divisor to be positive
		int parts = luaL_checkinteger(state, 2);
		lux_argcheck(state, 2, 0 < parts);
		// Force a complete division
		auto fract = div(size, parts);
		lux_argcheck(state, 2, fract.rem == 0);
		// Put data in new table
		User *data = user->data;
		lua_newtable(state);
		// Calculate each sub-array boundary
		for (int item = 1; item <= parts; ++item)
		{
			// Indexed from 1 like array
			lua_pushinteger(state, item);
			// Push sub-array onto the stack
			user->push(state, data, fract.quot, 1);
			// Put it into the table
			lua_settable(state, -3);
			// Advance to next
			data += fract.quot;
		}
		return 1;
	}

	/// Partition equally into sub-arrays
	static int __div(lua_State *state)
	{
		Type *user = Type::check(state, 1);
		// Pointer division forbidden
		int size = abs(user->size);
		lux_argcheck(state, 1, 0 < size);
		// Force divisor to be positive
		int parts = luaL_checkinteger(state, 2);
		lux_argcheck(state, 2, 0 < parts);
		// Force complete division
		auto fract = div(size, parts);
		lux_argcheck(state, 2, fract.rem == 0);
		// Put data in new table
		User *data = user->data;
		lua_newtable(state);
		// Calculate each sub-array boundary
		for (int item = 1; item <= fract.quot; ++item)
		{
			// Indexed from 1 like array
			lua_pushinteger(state, item);
			// Push sub-array onto the stack
			user->push(state, data, parts, 1);
			// Put it into the table
			lua_settable(state, -3);
			// Advance to next
			data += parts;
		}
		return 1;
	}

	/// Partition into overlapping areas
	static int __mod(lua_State *state)
	{
		Type *user = Type::check(state, 1);
		// Pointers not supported
		int size = abs(user->size);
		lux_argcheck(state, 1, 0 < size);
		// Partition must be within range
		int parts = luaL_checkinteger(state, 2);
		lux_argcheck(state, 2, 0 < parts);
		lux_argcheck(state, 2, parts < size);
		// Setup iterator and size
		auto data = user->data;
		size -= parts;
		// Store in a table
		lua_newtable(state);
		while (--parts) user->push(state, data++, size, 1);
		return 1;
	}

	/// Pointer addition arithmetic
	static int __add(lua_State *state)
	{
		Type *user = Type::check(state, 1);
		int offset = luaL_checkinteger(state, 2);
		int size = abs(user->size);
		if (0 < size) // is array
		{
			// Force offset to be positive
			lux_argcheck(state, 2, 0 <= offset);
			// Ensure that we stay in array range
			lux_argcheck(state, 2, offset < size);
			// Shrink range
			size -= offset;
			// Put referenced array on the stack
			user->push(state, user->data + offset, size, 1);
		}
		else // is pointer
		{
			// Put adjusted pointer on the stack
			Type::push(state, user->data + offset);
		}
		return 1;
	}

	/// Pointer subtraction arithmetic
	static int __sub(lua_State *state)
	{
		Type *user = Type::check(state, 1);
		int offset = luaL_checkinteger(state, 2);
		int size = abs(user->size);
		if (0 < size) // is array
		{
			// Force offset to be positive
			lux_argcheck(state, 2, 0 <= offset);
			// Ensure that we stay in array range
			lux_argcheck(state, 2, offset < size);
			// Shrink range
			size -= offset;
			// Put referenced array on the stack
			user->push(state, user->data, size, 1);
		}
		else // is pointer
		{
			// Put adjusted pointer on the stack
			Type::push(state, user->data - offset);
		}
		return 1;
	}

	/// Shift elements left by given amount
	static int __shl(lua_State *state)
	{
		Type *user = Type::check(state, 1);
		// Pointer shifting forbidden
		int size = abs(user->size);
		lux_argcheck(state, 1, 0 < size);
		// Accept only positive values for shift
		int shift = luaL_checkinteger(state, 2);
		lux_argcheck(state, 2, 0 < shift);
		// Proportions used in shift
		int n = shift % size;
		int m = size - n;
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

	/// Shift elements right by given amount
	static int __shr(lua_State *state)
	{
		Type *user = Type::check(state, 1);
		// Pointer shifting forbidden
		int size = abs(user->size);
		lux_argcheck(state, 1, 0 < size);
		// Accept only positive values for shift
		int shift = luaL_checkinteger(state, 2);
		lux_argcheck(state, 2, 0 < shift);
		// Proportions used in shift
		int n = shift % size;
		int m = size - n;
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

	/// Reverse elements in the array
	static int __unm(lua_State *state)
	{
		Type *user = Type::check(state);
		// Pointer reverse forbidden
		int size = abs(user->size);
		lux_argcheck(state, 1, 0 < size);
		// Half size of array
		int half = size >> 1;
		User temp; // for swap element
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

	/// Check equality of elements
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
		int size = abs(one->size);
		// Equal if, and only if, all the bits are identical
		if (memcmp(one->data, two->data, size*sizeof(User)) not_eq 0)
		{
			lux_push(state, false);
			return 1;
		}
		// Then one == two
		lux_push(state, true);
		return 1;
	}

	/// Lexical less than comparison
	static int __lt(lua_State *state)
	{
		Type *one = Type::check(state, 1);
		Type *two = Type::check(state, 2);
		// Genuine array sizes
		int n = abs(one->size);
		int m = abs(two->size);
		// Determine the smallest
		int size = n < m ? n : m;
		// Compare each respective element
		for (int item = 0; item < size; ++item)
		{
			// Return on first negative result
			if (two->data[item] <= one->data[item])
			{
				lux_push(state, false);
				return 1;
			}
		}
		// Then one < two
		lux_push(state, true);
		return 1;
	}

	/// Lexical less than or equal
	static int __le(lua_State *state)
	{
		Type *one = Type::check(state, 1);
		Type *two = Type::check(state, 2);
		// Genuine array sizes
		int n = abs(one->size);
		int m = abs(two->size);
		// Determine the smallest
		int size = n < m ? n : m;
		// Compare each respective element
		for (int item = 0; item < size; ++item)
		{
			// Return on first negative result
			if (two->data[item] < one->data[item])
			{
				lux_push(state, false);
				return 1;
			}
		}
		// Then one <= two
		lux_push(state, true);
		return 1;
	}

	// Element comparison usable by qsort and friends
	static int compare(const void *p1, const void *p2)
	{
		const User &a = *(const User *) p1;
		const User &b = *(const User *) p2;
		return a < b ? -1 : b < a ? +1 : 0;
	};

	/// Quick sort of the array elements
	static int sort(lua_State *state)
	{
		Type *user = Type::check(state, 1);
		// Pointer sorting forbidden
		size_t size = abs(user->size);
		lux_argcheck(state, 1, 0 < size);
		// Quick-sort algorithm in standard C library
		qsort(user->data, size, sizeof(User), compare);
	}

	/// Search a sorted array for first match
	static int search(lua_State *state)
	{
		Type *user = Type::check(state, 1);
		// Pointer indexing forbidden
		size_t size = abs(user->size);
		lux_argcheck(state, 1, 0 < size);
		// Find first index of this element
		User key = lux_to<User>(state, 2);
		// Standard C library 'bsearch'
		union { void *find; User *data; };
		find = bsearch(&key, user->data, size, sizeof(User), compare);
		// Index (from 1) of first element in array if found, else nil
		if (find) lua_pushinteger(state, user->data - data + 1);
		else lua_pushnil(state);
		return 1;
	}

	/// Write binary elements to a file
	static int write(lua_State *state)
	{
		Type *user = Type::check(state, 1);
		FILE *file = lux_opt(state, 2, stdout);
		// Using fwrite on given file
		int size = abs(user->size);
		size = fwrite(user->data, size, sizeof(User), file);
		lux_push(state, size);
		return 1;
	}

	/// Read binary elements from a file
	static int read(lua_State *state)
	{
		Type *user = Type::check(state, 1);
		FILE *file = lux_opt(state, 2, stdin);
		// Using fread on given file
		int size = abs(user->size);
		size = fread(user->data, size, sizeof(User), file);
		lux_push(state, size);
		return 1;
	}

	/// Write as string to a file
	static int puts(lua_State *state)
	{
		Type *user = Type::check(state, 1);
		FILE *file = lux_opt(state, 2, stdout);
		// Store UTF-8 characters
		int size = abs(user->size);
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

	/// Read as string from a file
	static int gets(lua_State *state)
	{
		Type *user = Type::check(state, 1);
		FILE *file = lux_opt(state, 2, stdin);
		// Store UTF-8 characters
		int size = abs(user->size);
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

	/// Copy array contents to another
	static int copy(lua_State *state)
	{
		Type *user = Type::check(state, 1);
		Type *to = Type::test(state, 2);
		// Second argument?
		if (!to)
		{
			int size = abs(user->size);
			User *data = new User [size];
			Type::push(state, data, size);
			// Copy contents of original to new array
			memcpy(data, user->data, size*sizeof(User));
			return 1;
		}
		else
		{
			int m = abs(to->size);
			int n = abs(user->size);
			int size = n < m ? n : m;
			// Copy contents over with possible overlap
			memmove(to->data, user->data, size*sizeof(User));
			return 0;
		}
	}

	/// Swap contents of two arrays
	static int swap(lua_State *state)
	{
		Type *user = Type::check(state, 1);
		Type *with = Type::check(state, 2);
		Type temp[1]; // temporary store
		// Swap content rather than data
		memcpy(temp, user, sizeof(Type));
		memcpy(user, with, sizeof(Type));
		memcpy(with, temp, sizeof(Type));
		return 0;
	}

	/// Loader compatible with luaL_requiref
	static int open(lua_State *state)
	{
		// Pull module name off the stack
		Type::name = lua_tostring(state, 1);
		// Go through the registry process once
		if (luaL_newmetatable(state, Type::name))
		{
			luaL_Reg regs [] =
			{
			{"sort", sort},
			{"search", search},
			{"write", write},
			{"read", read},
			{"puts", puts},
			{"gets", gets},
			{"copy", copy},
			{"swap", swap},
			{"new", __new},
			{"__gc", __gc},
			{"__tostring", __tostring},
			{"__concat", __concat},
			{"__newindex", __newindex},
			{"__index", __index},
			{"__len", __len},
			{"__mul", __mul},
			{"__div", __div},
			{"__add", __add},
			{"__sub", __sub},
			{"__shl", __shl},
			{"__shr", __shr},
			{"__unm", __unm},
			{"__eq", __eq},
			{"__lt", __lt},
			{"__le", __le},
			{nullptr}
			};
			// Register these functions
			luaL_setfuncs(state, regs, 0);
		}
		return 1;
	}
};

#endif // file

