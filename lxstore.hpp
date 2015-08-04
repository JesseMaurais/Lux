#ifndef __lxstore__
#define __lxstore__

#include "lxalloc.hpp"
#include <typeinfo>
#include <cstdlib>
#include <cassert>

// Metadata for type system

template <class User> struct lux_Type
{
	static const char *name;
};

template <class User> const char *lux_Type<User>::name = typeid(User).name();

// The base case

template <class User> struct lux_Pack : lux_Type<User>
{
	User data;

	lux_Pack(void) = default;

	lux_Pack(User &data)
	{
		this->data = data;
	}
};

// The pointer case

template <class User> struct lux_Pack<User*> : lux_Type<User*>
{
	User *data;
	ssize_t size;
	int ref;

	lux_Pack(User *data=nullptr, ssize_t size=0)
	{
		this->data = data;
		this->size = size;
		ref = LUA_NOREF;
	}
};

// The const pointer case

template <class User> struct lux_Pack<const User*> : lux_Pack<User*>
{
	lux_Pack(const User *data=nullptr, ssize_t size=0)
	{
		this->data = const_cast<User*>(data); // Dangerous?
		this->size = size;
		this->ref = LUA_NOREF;
	}
};

// The fixed sized array case

template <class User, size_t Size> struct lux_Pack<User[Size]> : lux_Pack<User*>
{
	lux_Pack(User *data=nullptr)
	{
		this->data = data;
		this->size = -Size; // Not owner -- note the sign
		this->ref = LUA_NOREF;
	}
};

// Basic stack operations for userdata

template <class User> struct lux_Store : lux_Pack<User>
{
	typedef lux_Store<User> Type;

	static Type *check(lua_State *state, int stack=1)
	{
		union {
		 void *address;
		 Type *user;
		};
		address = luaL_checkudata(state, stack, Type::name);
		return user;
	}

	static Type *test(lua_State *state, int stack=1)
	{
		union {
		 void *address;
		 Type *user;
		};
		address = luaL_testudata(state, stack, Type::name);
		return user;
	}

	static Type *push(lua_State *state, User data, ssize_t size=0)
	{
		auto user = new (state) Type(data, size);
		luaL_setmetatable(state, Type::name);
		return user;
	}

	static User opt(lua_State *state, int stack, User opt)
	{
		auto user = test(state, stack);
		return user ? user->data : opt;
	}

	static User to(lua_State *state, int stack=1)
	{
		return check(state, stack)->data;
	}

	using lux_Pack<User>::lux_Pack;
};

// Specialize for pointers -- convert between nullptr/nil

template <class User> struct lux_Store<User*> : lux_Pack<User*>
{
	typedef lux_Store<User*> Type;

	static Type *check(lua_State *state, int stack=1)
	{
		union {
		 void *address;
		 Type *user;
		};
		address = luaL_checkudata(state, stack, Type::name);
		return user;
	}

	static Type *test(lua_State *state, int stack=1)
	{
		union {
		 void *address;
		 Type *user;
		};
		address = luaL_testudata(state, stack, Type::name);
		return user;
	}

	static Type *push(lua_State *state, User *data, ssize_t size=0)
	{
		if (data)
		{
			auto user = new (state) Type(data, size);
			luaL_setmetatable(state, Type::name);
			return user;
		}
		lua_pushnil(state);
		return nullptr;
	}

	static User *opt(lua_State *state, int stack=1, User *opt=nullptr)
	{
		auto user = test(state, stack);
		return user ? user->data : opt;
	}

	static User *to(lua_State *state, int stack=1)
	{
		if (lua_isnoneornil(state, stack)) return nullptr;
		return check(state, stack)->data;
	}

	// Push with reference to data owner where this is at stack index
	Type* push(lua_State *state, User *data, size_t size, int stack)
	{
		// Put data on the stack (not as owner)
		auto user = push(state, data, - size);
		// Not null and is an array
		if (user and this->size not_eq 0)
		{
			// Begin pointer in range
			assert(this->data <= data);
			// End pointer is also in range
			assert(this->data + abs(this->size) >= data + size);
			// Store reference numbers in the metatable
			luaL_getmetatable(state, Type::name);
			// This is data owner?
			if (this->size < 0)
			{
			// Reference the owner instead
			lua_rawgeti(state, -1, this->ref);
			}
			else
			{
			// Copy array to reference
			lua_pushvalue(state, stack);
			}
			// Create a unique reference
			user->ref = luaL_ref(state, -2);
			// Remove metatable
			lua_pop(state, 1);
		}
		return user;
	}

	using lux_Pack<User*>::lux_Pack;
};

/* Special care is taken for pointers to data types since we want to have a
 * common interface for strings, arrays, and pointers the way that C treats
 * them but we want Lua code to be guarded against common errors in pointer
 * arithmetic, like segfaults. Also, Lua is garbage collected so we have to
 * maintain references to the original data for sub-arrays which point into
 * true arrays.
 */

#endif // file

