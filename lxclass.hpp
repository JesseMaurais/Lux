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

	/// Get temporary (no-ref) pointer
	static int __call(lua_State *state)
	{
		Type *user = Type::check(state, 1);
		// Pointer index prohibited
		int size = abs(user->size);
		lux_argcheck(state, 1, 0 < size);
		// Check that object index is in range
		int index = luaL_checkinteger(state, 2);
		lux_argcheck(state, 2, 0 < index);
		lux_argcheck(state, 2, index <= size);
		// Put pointer on the stack
		-- index; // C indexes from 0, not 1
		Type::push(state, user->data + index);
		return 1;
	}

	/// String conversion for printing
	static int __tostring(lua_State *state)
	{
		lua_pushfstring(state, "%s: %p", Type::name, Type::to(state));
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
		// Pull module name off the stack
		Type::name = lua_tostring(state, 1);
		// Go through registry process only once
		if (luaL_newmetatable(state, Type::name))
		{
			luaL_Reg regs[] =
			{
			{"new", __new},
			{"__gc", __gc},
			{"__len", __len},
			{"__call", __call},
			{"__tostring", __tostring},
			{nullptr}
			};
			luaL_setfuncs(state, regs, 0);
			if (index->name)
			{
			lua_pushliteral(state, "__index");
			luaL_newlib(state, index);
			lua_settable(state, -3);
			}
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

