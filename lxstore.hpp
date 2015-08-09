#ifndef __lxstore__
#define __lxstore__

/**
 * Storage classes are a thin layer around C data types which map them onto
 * Lua metatables (logically a many-to-one mapping).
 *
 * Special care is taken for pointers to data types since we want to have a
 * common interface for strings, arrays, and pointers the way that C treats
 * them but we want Lua code to be guarded against common errors in pointer
 * arithmetic, like segfaults. Also, Lua is garbage collected so we have to
 * maintain references to the original data for sub arrays which point into
 * owning arrays. All such information is stored in the 'size' field of our
 * storage class. We use a signed size type so that the sign signifies that
 * the data is owned by this storage (when size > 0) whether it is refering
 * to another data owner (when size < 0) or whether it points into a system
 * designated memory area (when size = 0). In the second case a 'ref' field
 * also refers to the data owner. The lux_Array class has implementated all
 * of these features and more for using C strings/arrays/pointers in Lua.
 */

#include "lxalloc.hpp"
#include <typeinfo>
#include <cstdlib>
#include <cassert>

/// Maps C types to Lua metatable names
template <class User> struct lux_Type
{
	static const char *name;
};

/// By default we will employ the C++ typeid name which is compiler dependent
template <class User> const char *lux_Type<User>::name = typeid(User).name();

/// Storage class for raw C user data types
template <class User> struct lux_Pack : lux_Type<User>
{
	User data;

	lux_Pack(void) = default;

	lux_Pack(User &data)
	{
		this->data = data;
	}
};

/// Storage class for the special case of pointers to C data
template <class User> struct lux_Pack<User*> : lux_Type<User*>
{
	User *data;
	int size;
	int ref;

	lux_Pack(User *data=nullptr, int size=0)
	{
		this->data = data;
		this->size = size;
		this->ref = LUA_NOREF;
	}
};

/// Storage class for const pointers inherits from raw pointers
template <class User> struct lux_Pack<const User*> : lux_Pack<User*>
{
	lux_Pack(const User *data=nullptr, int size=0)
	{
		this->data = const_cast<User*>(data); // Dangerous?
		this->size = size;
		this->ref = LUA_NOREF;
	}
};

/// Storage class for fixed size arrays inherits from pointers with caveats
template <class User, int Size> struct lux_Pack<User[Size]> : lux_Pack<User*>
{
	lux_Pack(User *data=nullptr)
	{
		this->data = data;
		this->size = - Size; // Not owner -- note the sign
		this->ref = LUA_NOREF;
	}
};

/// Basic stack operations for C user data (base case)
template <class User> struct lux_Store : lux_Pack<User>
{
	typedef lux_Store<User> Type; // storage class

	/// Return storage if types match or not at all
	static Type *check(lua_State *state, int stack=1)
	{
		union {
		 void *address;
		 Type *user;
		};
		address = luaL_checkudata(state, stack, Type::name);
		return user;
	}

	/// Return storage if types match or nullptr
	static Type *test(lua_State *state, int stack=1)
	{
		union {
		 void *address;
		 Type *user;
		};
		address = luaL_testudata(state, stack, Type::name);
		return user;
	}

	/// Create storage for user data and put it on the stack
	static Type *push(lua_State *state, User data, int size=0)
	{
		auto user = new (state) Type(data);
		luaL_setmetatable(state, Type::name);
		return user;
	}

	/// Return data if types match or the given default
	static User &opt(lua_State *state, int stack, User &opt)
	{
		auto user = test(state, stack);
		return user ? user->data : opt;
	}

	/// Return data if types match or not at all
	static User &to(lua_State *state, int stack=1)
	{
		return check(state, stack)->data;
	}

	/// Inherit base constructor
	using lux_Pack<User>::lux_Pack;
};

/// Specialized for pointers with conversion between nullptr/nil
template <class User> struct lux_Store<User*> : lux_Pack<User*>
{
	typedef lux_Store<User*> Type; // storage class

	/// Return storage if types match or not at all
	static Type *check(lua_State *state, int stack=1)
	{
		union {
		 void *address;
		 Type *user;
		};
		address = luaL_checkudata(state, stack, Type::name);
		return user;
	}

	/// Return storage if types match or nullptr
	static Type *test(lua_State *state, int stack=1)
	{
		union {
		 void *address;
		 Type *user;
		};
		address = luaL_testudata(state, stack, Type::name);
		return user;
	}

	/// Create storage for user data and put it on the stack
	static Type *push(lua_State *state, User *data, int size=0)
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

	/// Return data if types match or the given default
	static User *opt(lua_State *state, int stack=1, User *opt=nullptr)
	{
		auto user = test(state, stack);
		return user ? user->data : opt;
	}

	/// Return data if types match or not at all
	static User *to(lua_State *state, int stack=1)
	{
		if (lua_isnoneornil(state, stack)) return nullptr;
		return check(state, stack)->data;
	}

	/// Push with reference to owner where 'this' is on the stack
	Type* push(lua_State *state, User *data, int size, int stack)
	{
		// Put data on the stack (not as owner)
		auto user = push(state, data, - size);
		// Not null and this is an array
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

	/// Inherit base constructor
	using lux_Pack<User*>::lux_Pack;
};

#endif // file

