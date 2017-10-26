#ifndef __lxclass__
#define __lxclass__

/**
 * Mimics some of the features of arrays but intended to store arrays of C++
 * objects like class/struct/union rather than POD types. Supports access to
 * fields within the objects as well as class methods using a metatable. It
 * does not support any of the manipulation functions for arrays.
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
		std::ptrdiff_t size = 1;
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
			// no break
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
		auto user = Type::check(state);
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
		auto user = Type::check(state);
		return lux_push(state, std::abs(user->size));
	}

	/// Get temporary (no-ref) pointer
	static int __call(lua_State *state)
	{
		auto user = Type::check(state, 1);
		// Pointer index prohibited
		auto size = std::abs(user->size);
		lux_argcheck(state, 1, 0 < size);
		// Check that object index is in range
		auto index = luaL_checkinteger(state, 2);
		lux_argcheck(state, 2, 0 < index);
		lux_argcheck(state, 2, index <= size);
		// Put pointer on the stack
		--index; // C indexes from 0, not 1
		user->push(state, user->data + index, 1, 1);
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
		auto data = Type::to(state);
		// Always return previous/current value
		lux_push(state, data->*field);
		// Change if argument given
		if (lua_gettop(state) > 1)
		{
			data->*field = lux_to<Base>(state, 2);
		}
		return 1;
	}

	/// Loader compatible with luaL_requiref
	static int open(lua_State *state)
	{
		// Pull module name off the stack
		auto name = lua_tostring(state, 1);
		// Go through registry process only once
		if (luaL_newmetatable(state, name))
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

	/// Require-like function joining 'name' and 'open'
	static void require(lua_State *state, bool global=false)
	{
		luaL_requiref(state, Type::name, open, global);
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

