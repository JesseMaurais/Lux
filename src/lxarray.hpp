#ifndef __lxarray__
#define __lxarray__

/**
 * This is the preferred implementation of C arrays and pointers for Lua. The
 * array is guaranteed to be a contiguous area of memory into which writing
 * and reading elements is done with the index metamethods. Several operations
 * on arrays are also provided through other metamethods. Creating sub-arrays
 * which point to some contiguous area within an array is also possible, where
 * changes in one array are also reflected automatically in the other. Memory
 * is properly reference counted in this case to avoid segfaults. Raw pointers
 * are implemented in this same metatable but the array functions are not.
 * Pointers are supported only for system related functions. 
 *
 * See lxstore.hpp for the storage classes and lxchars.hpp for char decoding.
 * See array.cpp for the C++ source code that compiles into the array module. 
 */

#include <cmath>
#include "lxalloc.hpp"
#include "lxstore.hpp"
#include "lxstack.hpp"
#include "lxbuffs.hpp"
#include "lxerror.hpp"

/// Emulate C strings/arrays/pointers in Lua with extra operations

template <class Type> struct lux_Array
{
	// Pointer storage implementation
	typedef lux_Store<Type*> Store;

	/// Array allocation function
	static int __new(lua_State *state)
	{
		std::ptrdiff_t size;
		Type *data;
		switch (lua_type(state, 1))
		{
		default:
			// We only construct from these argument types
			return luaL_argerror(state, 1, "size, table, array");
		case LUA_TNUMBER:
			// Argument is the array's size
			size = lua_tointeger(state, 1);
			// Check that the size is valid
			lux_argcheck(state, 1, 0 < size);
			// Create its storage
			data = new Type [size];
			break;
		case LUA_TTABLE:
			// Copy table contents
			size = luaL_len(state, 1);
			data = new Type [size];
			lua_pushnil(state); // first
			// Copy every element of the table as numeric
			for (int item = 0; lua_next(state, 1); ++item)
			{
				// Convert the element to User type
				data[item] = lux_to<Type>(state, 3);
				lua_pop(state, 1); // pop key
			}
			break;
		case LUA_TUSERDATA:
			// Copied from an array
			size = luaL_len(state, 1);
			data = new Type [size];
			// Use the index metamethod for generality
			for (decltype(size) i = 0, j = 1; i < size; i = j++)
			{
				lua_geti(state, 1, j);
				data[i] = lux_to<Type>(state, -1);
				lua_pop(state, 1);
			}
			break;
		case LUA_TLIGHTUSERDATA:
			// Implicit conversion from a raw pointer
			data = static_cast<Type*>(lua_touserdata(state, 1));
			size = 0;
			break;
		};
		Store::push(state, data, size);
		return 1;
	}

	/// Garbage collection callback
	static int __gc(lua_State *state)
	{
		auto user = Store::check(state);
		// Only the owner has non-negative size
		if (0 < user->size) delete [] user->data;
		else
		{
			luaL_getmetatable(state, Store::name);
			// Free reference to the data owner
			luaL_unref(state, -1, user->ref);
		}
		return 0;
	}

	/// Size of array or zero if pointer
	static int __len(lua_State *state)
	{
		auto user = Store::check(state);
		return lux_push(state, std::abs(user->size));
	}

	/// Read array data at offset
	static int __index(lua_State *state)
	{
		auto user = Store::check(state, 1);
		// Pointers not supported
		auto size = std::abs(user->size);
		lux_argcheck(state, 1, 0 < size);
		// Ensure we stay in array bounds
		auto index = luaL_checkinteger(state, 2);
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
		auto user = Store::check(state);
		// Pointers not supported
		auto size = std::abs(user->size);
		lux_argcheck(state, 1, 0 < size);
		// Ensure we stay in array bounds
		auto index = luaL_checkinteger(state, 2);
		if (0 < index and index <= size)
		{
			// Arrays are indexed from 1 rather than 0
			user->data[--index] = lux_to<Type>(state, 3);
		}
		return 0;
	}

	/// String conversion for printing
	static int __tostring(lua_State *state)
	{
		auto user = Store::check(state);
		auto size = std::abs(user->size);
		if (0 == size) // pointer
		{
			// Put formatted string of metatable name and address
			lua_pushfstring(state, "%s: %p", Store::name, user->data);
			return 1;
		}
		// Build up a string
		lux_Buffer buffer(state);
		bool meta = true; // try metamethod
		// Iterate over each item in the array
		for (decltype(size) item = 0; item < size; ++item)
		{
			// Add separator only after first
			if (0 == item) buffer.addstring(", ");
			// Convert each element to string
			lux_push(state, user->data[item]);
			if (meta) // use __tostring
			{
				if (luaL_callmeta(state, -1, "__tostring"))
				{
					buffer.addvalue(); // add string
					lua_pop(state, 1); // pop value
					continue;
				}
				meta = false;
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
		auto A = Store::check(state, 1);
		auto B = Store::check(state, 2);
		// Genuine array sizes
		auto n = std::abs(A->size);
		auto m = std::abs(B->size);
		// Neither pointers
		if (n > 0 and m > 0)
		{
			auto size = n + m;
			auto data = new Type [size];
			// Copy contents of input arrays
			std::memcpy(data, A->data, n * sizeof(Type));
			std::memcpy(data + n, B->data, m * sizeof(Type));
			// Put array onto the stack
			Store::push(state, data, size);
			return 1;
		}
		return 0;
	}

	/// Partition into equal sub-arrays
	static int __mul(lua_State *state)
	{
		auto user = Store::check(state, 1);
		// Pointers not supported
		auto size = std::abs(user->size);
		lux_argcheck(state, 1, 0 < size);
		// Divisor must be positive
		auto parts = luaL_checkinteger(state, 2);
		lux_argcheck(state, 2, 0 < parts);
		// Division must be complete
		auto div = std::lldiv(size, parts);
		lux_argcheck(state, 2, div.rem == 0);
		// Put data in new table
		auto data = user->data;
		lua_newtable(state);
		// Calculate each sub-array boundary
		for (decltype(parts) item = 1; item <= parts; ++item)
		{
			// Indexed from 1 like array
			lua_pushinteger(state, item);
			// Push sub-array onto the stack
			user->push(state, data, div.quot, 1);
			// Put it into the table
			lua_settable(state, -3);
			// Advance to next
			data += div.quot;
		}
		return 1;
	}

	/// Partition equally into sub-arrays
	static int __div(lua_State *state)
	{
		auto user = Store::check(state, 1);
		// Pointers not supported
		auto size = std::abs(user->size);
		lux_argcheck(state, 1, 0 < size);
		// Divisor must be positive
		auto parts = luaL_checkinteger(state, 2);
		lux_argcheck(state, 2, 0 < parts);
		// Division must be complete
		auto div = std::lldiv(size, parts);
		lux_argcheck(state, 2, div.rem == 0);
		// Put data in new table
		auto data = user->data;
		lua_newtable(state);
		// Calculate each sub-array boundary
		for (decltype(div.quot) item = 1; item <= div.quot; ++item)
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

	/// Pointer addition arithmetic
	static int __add(lua_State *state)
	{
		auto user = Store::check(state, 1);
		auto offset = luaL_checkinteger(state, 2);
		auto size = std::abs(user->size);
		if (0 < size) // is array
		{
			// Offset must be non negative
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
			Store::push(state, user->data + offset);
		}
		return 1;
	}

	/// Pointer subtraction arithmetic
	static int __sub(lua_State *state)
	{
		auto user = Store::check(state, 1);
		auto offset = luaL_checkinteger(state, 2);
		auto size = std::abs(user->size);
		if (0 < size) // is array
		{
			// Offset must be non negative
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
			Store::push(state, user->data - offset);
		}
		return 1;
	}

	/// Rotate elements left by given amount
	static int __shl(lua_State *state)
	{
		auto user = Store::check(state, 1);
		// Pointers not supported
		auto size = std::abs(user->size);
		lux_argcheck(state, 1, 0 < size);
		// Accept only positive values for shift
		auto shift = luaL_checkinteger(state, 2);
		lux_argcheck(state, 2, 0 < shift);
		// Proportions used in shift
		auto n = shift % size;
		auto m = size - n;
		// Swap memory
		if (n < m)
		{
			Type temp[m]; // Copy to avoid overwrite
			std::memcpy(temp, user->data + n, m*sizeof(Type));
			std::memcpy(user->data + m, user->data, n*sizeof(Type));
			std::memcpy(user->data, temp, m*sizeof(Type));
		}
		else
		{
			Type temp[n]; // Copy to avoid overwrite
			std::memcpy(temp, user->data, n*sizeof(Type));
			std::memcpy(user->data, user->data + n, m*sizeof(Type));
			std::memcpy(user->data + m, temp, n*sizeof(Type));
		}
		// Return a reference
		lua_pushvalue(state, 1);
		return 1;
	}

	/// Rotate elements right by given amount
	static int __shr(lua_State *state)
	{
		auto user = Store::check(state, 1);
		// Pointers not supported
		auto size = std::abs(user->size);
		lux_argcheck(state, 1, 0 < size);
		// Accept only positive values for shift
		auto shift = luaL_checkinteger(state, 2);
		lux_argcheck(state, 2, 0 < shift);
		// Proportions used in shift
		auto n = shift % size;
		auto m = size - n;
		// Swap memory
		if (m < n)
		{
			Type temp[n]; // Copy to avoid overwrite
			std::memcpy(temp, user->data + m, n*sizeof(Type));
			std::memcpy(user->data + n, user->data, m*sizeof(Type));
			std::memcpy(user->data, temp, n*sizeof(Type));
		}
		else
		{
			Type temp[m]; // Copy to avoid overwrite
			std::memcpy(temp, user->data, m*sizeof(Type));
			std::memcpy(user->data, user->data + m, n*sizeof(Type));
			std::memcpy(user->data + n, temp, m*sizeof(Type));
		}
		// Return a reference
		lua_pushvalue(state, 1);
		return 1;
	}

	/// Convert to a raw pointer
	static int __unm(lua_State *state)
	{
		lua_pushlightuserdata(state, Store::to(state));
		return 1;
	}

	/// Determine if array is not owner
	static int __bnot(lua_State *state)
	{
		auto user = Store::check(state);
		lua_pushboolean(state, std::signbit(user->size));
		return 1;
	}

	/// Determine if this is a sub array
	static int __band(lua_State *state)
	{
		// 1st array
		auto A = Store::check(state, 1);
		auto A_head = A->data;
		auto A_tail = A->data + std::abs(A->size);
		// 2nd array
		auto B = Store::check(state, 2);
		auto B_head = B->data;
		auto B_tail = B->data + std::abs(B->size);
		// Boundary of second fits entirely within boundary of first
		lua_pushboolean(state, A_head <= B_head and B_tail <= A_tail);
		return 1;
	}

	/// Determine if there is no overlap
	static int __bxor(lua_State *state)
	{
		// 1st array
		auto A = Store::check(state, 1);
		auto A_head = A->data;
		auto A_tail = A->data + std::abs(A->size);
		// 2nd array
		auto B = Store::check(state, 2);
		auto B_head = B->data;
		auto B_tail = B->data + std::abs(B->size);
		// Head of first is beyond tail of second or the converse
		lua_pushboolean(state, A_tail <= B_head or B_tail <= A_head);
		return 1;
	}

	/// Determine if there is some overlap
	static int __bor(lua_State *state)
	{
		// 1st array
		auto A = Store::check(state, 1);
		auto A_head = A->data;
		auto A_tail = A->data + std::abs(A->size);
		// 2nd array
		auto B = Store::check(state, 2);
		auto B_head = B->data;
		auto B_tail = B->data + std::abs(B->size);
		// Head of one falls within the boundary of the other
		if (A_head <= B_head)
			lua_pushboolean(state, B_head < A_tail);
		else
			lua_pushboolean(state, A_head < B_tail);
		return 1;
	}

	/// Check equality of elements
	static int __eq(lua_State *state)
	{
		auto A = Store::check(state, 1);
		auto B = Store::check(state, 2);
		// Not equal if sizes aren't
		if (std::abs(A->size) != std::abs(B->size))
		{
			lux_push(state, false);
			return 1;
		}
		// Are equal if pointers are
		if (A->data == B->data)
		{
			lux_push(state, true);
			return 1;
		}
		auto size = std::abs(A->size);
		// Equal if, and only if, all the bits are identical
		if (std::memcmp(A->data, B->data, size*sizeof(Type)))
		{
			lux_push(state, false);
			return 1;
		}
		// Then A == B
		lux_push(state, true);
		return 1;
	}

	/// Lexical less than comparison
	static int __lt(lua_State *state)
	{
		auto A = Store::check(state, 1);
		auto B = Store::check(state, 2);
		// Genuine array sizes
		auto n = std::abs(A->size);
		auto m = std::abs(B->size);
		// Determine the smallest
		auto size = std::min(n, m);
		// Compare each respective element
		for (decltype(size) item = 0; item < size; ++item)
		{
			// Return on first negative result
			if (A->data[item] <= B->data[item])
			{
				lux_push(state, false);
				return 1;
			}
		}
		// Then A < B
		lux_push(state, true);
		return 1;
	}

	/// Lexical less than or equal
	static int __le(lua_State *state)
	{
		auto A = Store::check(state, 1);
		auto B = Store::check(state, 2);
		// Genuine array sizes
		auto n = std::abs(A->size);
		auto m = std::abs(B->size);
		// Determine the smallest
		auto size = n < m ? n : m;
		// Compare each respective element
		for (decltype(size) item = 0; item < size; ++item)
		{
			// Return on first negative result
			if (A->data[item] < B->data[item])
			{
				lux_push(state, false);
				return 1;
			}
		}
		// Then A <= B
		lux_push(state, true);
		return 1;
	}

	// Element comparison usable by qsort and friends
	static int compare(const void *p1, const void *p2)
	{
		const auto &a = *(const Type *) p1;
		const auto &b = *(const Type *) p2;
		return a < b ? -1 : b < a ? +1 : 0;
	}

	// Quick sort of the array elements
	static int sort(lua_State *state)
	{
		auto user = Store::check(state);
		// Pointers not supported
		auto size = std::abs(user->size);
		lux_argcheck(state, 1, 0 < size);
		// Quick-sort algorithm from standard C library
		std::qsort(user->data, size, sizeof(Type), compare);
		return 0;
	}

	/// Search a sorted array for first match
	static int search(lua_State *state)
	{
		auto user = Store::check(state, 1);
		// Pointers not supported
		auto size = std::abs(user->size);
		lux_argcheck(state, 1, 0 < size);
		// Find some index of this element
		auto key = lux_to<Type>(state, 2);
		// Binary search in standard C
		union { void *find; Type *data; };
		find = std::bsearch(&key, user->data, size, sizeof(Type), compare);
		// Index (from 1) of first element in array if found, else nil
		if (find) lua_pushinteger(state, user->data - data + 1);
		else lua_pushnil(state);
		return 1;
	}

	/// Write binary elements to a file
	static int write(lua_State *state)
	{
		auto user = Store::check(state, 1);
		auto file = lux_opt(state, 2, stdout);
		// Using fwrite on given file
		auto size = std::abs(user->size);
		size = std::fwrite(user->data, size, sizeof(Type), file);
		return lux_push(state, size);
	}

	/// Read binary elements from a file
	static int read(lua_State *state)
	{
		auto user = Store::check(state, 1);
		auto file = lux_opt(state, 2, stdin);
		// Using fread on given file
		auto size = std::abs(user->size);
		size = std::fread(user->data, size, sizeof(Type), file);
		return lux_push(state, size);
	}

	/// Set bits in sub array to zero
	static int zero(lua_State *state)
	{
		auto user = Store::check(state, 1);
		auto size = std::abs(user->size);
		// Region within array to zero the bits
		auto begin = luaL_optinteger(state, 2, 1);
		auto end = luaL_optinteger(state, 3, size + 1);
		// Check the sanity of the region
		lux_argcheck(state, 2, begin < end);
		size = end - begin;
		--begin;
		// Fastest standard way to zero contiguous bits
		std::memset(user->data + begin, 0, size*sizeof(Type));
		return 0;
	}

	/// Swap contents of two arrays
	static int swap(lua_State *state)
	{
		auto A = Store::check(state, 1);
		auto B = Store::check(state, 2);
		// Optional argument defaults to size of first
		auto size = lux_opt(state, 3, std::abs(A->size));
		lux_argcheck(state, 1, 0 < size);
		// Buffer user's contents during swap
		Type temp[size];
		std::memcpy(temp, A->data, sizeof(temp));
		std::memcpy(A->data, B->data, sizeof(temp));
		std::memcpy(B->data, temp, sizeof(temp));
		return 0;
	}

	/// Copy array contents to new array
	static int copy(lua_State *state)
	{
		auto user = Store::check(state, 1);
		// Pointers not supported
		auto size = std::abs(user->size);
		lux_argcheck(state, 1, 0 < size);
		// Create a new buffer and copy
		auto data = new Type [size];
		std::memcpy(data, user->data, size*sizeof(Type));
		// Put new array on the stack
		Store::push(state, data, size);
		return 1;
	}

	/// Move contents to existing array
	static int move(lua_State *state)
	{
		auto user = Store::check(state, 1);
		// Pointers not supported
		auto size = std::abs(user->size);
		lux_argcheck(state, 1, 0 < size);
		// Region in array to be moved
		auto begin = luaL_optinteger(state, 2, 1);
		lux_argcheck(state, 2, 0 < begin);
		auto end = luaL_optinteger(state, 3, size);
		lux_argcheck(state, 3, end <= size);
		auto to = luaL_optinteger(state, 4, 1);
		// Write to same if argument absent
		auto target = Store::test(state, 5);
		if (not target) target = user;
		size = std::abs(target->size);
		lux_argcheck(state, 4, to <= size);
		// Adjust metrics
		--begin, --end, --to;
		size = end - begin;
		// Use memmove in case we have overlapping memory areas
		std::memmove(target->data + to, user->data + begin, size*sizeof(Type));
		return 0;
	}

	/// Create array from arguments
	static int pack(lua_State *state)
	{
		auto size = lua_gettop(state);
		auto data = new Type [size];
		for (decltype(size) item = 0; item < size; ++item)
		{
			data[item] = lux_to<Type>(state, item+1);
		}
		Store::push(state, data, size);
		return 1;
	}

	/// Push all elements on the stack
	static int unpack(lua_State *state)
	{
		auto user = Store::check(state, 1);
		auto size = std::abs(user->size);
		lux_argcheck(state, 1, 0 < size);
		auto begin = luaL_optinteger(state, 2, 1);
		lux_argcheck(state, 2, 0 < begin);
		auto end = luaL_optinteger(state, 3, size);
		lux_argcheck(state, 3, end <= size);
		for (decltype(size) item = --begin; item < end; ++item)
		{
			lux_push(state, user->data[item]);
		}
		return static_cast<int>(end - begin);
	}

	/// Loader compatible with luaL_requiref
	static int open(lua_State *state)
	{
		// Pull module name off the stack
		auto name = lua_tostring(state, 1);
		// Create a metatable for array
		luaL_newmetatable(state, name);
		{
			luaL_Reg regs [] =
			{
			{"write", write},
			{"read", read},
			{"zero", zero},
			{"swap", swap},
			{"copy", copy},
			{"move", move},
			{"pack", pack},
			{"unpack", unpack},
			{"search", search},
			{"sort", sort},
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
			{"__bnot", __bnot},
			{"__band", __band},
			{"__bxor", __bxor},
			{"__bor", __bor},
			{"__eq", __eq},
			{"__lt", __lt},
			{"__le", __le},
			{nullptr}
			};
			// Register these functions
			luaL_setfuncs(state, regs, 0);
		}
		// Done
		return 1;
	}
	
	/// Require-like function joining 'name' and 'open'
	static void require(lua_State *state, bool global=false)
	{
		luaL_requiref(state, Store::name, open, global);
	}
};

#endif // file

