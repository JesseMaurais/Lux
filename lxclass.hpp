#ifndef __lxclass__
#define __lxclass__

/**
 * Mimics some of the features of arrays but intended to store arrays of C++
 * objects like class/struct/union rather than POD types. Supports access to
 * fields within the objects (their members) as well as class methods in the
 * metatable.
 */

#include "lxalloc.hpp"
#include "lxstore.hpp"
#include "lxstack.hpp"

/// Export C++ classes/structs/unions to Lua

template <class User> struct lux_Class
{
	// Pointer storage implementation
	typedef lux_Store<User*> Type;

	/// Array allocation function
	static int __new(lua_State *state)
	{
		int size = 1;
		User *data = nullptr;
		// Check constructor argument
		switch (lua_type(state, 1))
		{
		  default:
			// We only construct from these argument types
			return luaL_argerror(state, 1, "none, number");
		  case LUA_TNUMBER:
			// array
			size = lua_tointeger(state, 1);
			lux_argcheck(state, 1, 0 < size);
		  case LUA_TNONE:
			// single
			data = new User [size];
		};
		Type::push(state, data, size);
		return 1;
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

	/// String conversion for printing
	static int __tostring(lua_State *state)
	{
		lua_pushfstring(state, "%s: %p", Type::name, Type::to(state));
		return 1;
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
		int items = luaL_checkinteger(state, 2);
		lux_argcheck(state, 2, 0 < items);
		// Force complete division
		auto fract = div(size, items);
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
			user->push(state, data, items, 1);
			// Put it into the table
			lua_settable(state, -3);
			// Advance to next
			data += items;
		}
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
		User temp[1]; // for swap element
		for (int i = 0, j = size - 1; i < half; ++i, --j)
		{
			memcpy(temp, user->data + i, sizeof(User));
			memcpy(user->data + i, user->data + j, sizeof(User));
			memcpy(user->data + j, temp, sizeof(User));
		}
		// Return a reference
		lua_pushvalue(state, 1);
		return 1;
	}

	/// Read/Write into given fields of records
	template <class Base> inline
	static int member(lua_State *state, Base User::*field)
	{
		User *data = lux_to<User*>(state, 1);
		// Write if argument given
		if (lua_gettop(state) > 1)
		{
		 data->*field = lux_to<Base>(state, 2);
		 return 0;
		}
		else // Read
		{
		 lux_push<Base>(state, data->*field);
		 return 1;
		}
	}

	/// Loader compatible with luaL_requiref
	static int open(lua_State *state)
	{
		Type::name = lua_tostring(state, +1);
		luaL_newmetatable(state, Type::name);
		luaL_Reg regs[] =
		{
		{"new", __new},
		{"__gc", __gc},
		{"__len", __len},
		{"__tostring", __tostring},
		{"__mul", __mul},
		{"__div", __div},
		{"__add", __add},
		{"__sub", __sub},
		{"__shl", __shl},
		{"__shr", __shr},
		{"__unm", __unm},
		{nullptr}
		};
		luaL_setfuncs(state, regs, 0);
		if (index->name)
		{
		lua_pushliteral(state, "__index");
		luaL_newlib(state, index);
		lua_settable(state, -3);
		}
		return 1;
	}

	/// Class methods index
	static luaL_Reg index[];
};

/// Assume an empty index unless users fill out the template otherwise
template <class User> luaL_Reg lux_Class<User>::index[] = {{nullptr}};

/// Wrap a member accessor in an anonymous function compatible with Lua
#define lux_member(C, x) \
	[](lua_State *state)->int { return lux_Class<C>::member(state, &C::x); }

/// Use a similar syntax for class methods
#define lux_method(C, x) lux_cast(C::x)


#endif // file

