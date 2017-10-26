#ifndef __lxstore__
#define __lxstore__

/**
 * Storage classes are a thin layer around C data types which map them onto
 * Lua metatables (logically a many-to-one mapping).
 *
 * Special care is taken for pointers to data types since we want to have
 * a common interface for strings, arrays, and pointers the same way that C
 * treats them but we want Lua code to be guarded against common errors in
 * pointer arithmetic. Also, Lua is garbage collected so we have to maintain
 * references to the original data for sub arrays which point into owning
 * arrays. All such information is stored in the 'size' field of our storage
 * class. We use a signed size type so that the sign signifies whether the
 * data is owned by this storage (when size > 0) or whether it is refering
 * to another data owner (when size < 0) or whether it points into a system
 * designated memory area (when size = 0). In the second case a 'ref' field
 * also refers to the data owner. The lux_Array class has implementated all
 * of these features and more for using C strings/arrays/pointers in Lua.
 */

#include "lxalloc.hpp"
#include <typeinfo>
#include <utility>
#include <cstddef>
#include <cstdlib>
#include <cassert>

/// Maps C types to Lua metatable names
template <class Type> struct lux_Name
{
	static const char *name;
};

/// By default we will employ the typeid name which is compiler dependent
template <class Type> const char *lux_Name<Type>::name = typeid(Type).name();

/// Storage class for raw C user data types
template <class Type> struct lux_Pack : lux_Name<Type>
{
	Type data;

	lux_Pack(void) = default;

	lux_Pack(Type &&data)
	{
		this->data = std::move(data);
	}

	lux_Pack(Type &data)
	{
		this->data = std::move(data);
	}
};

/// Storage class for the special case of pointers to C data
template <class Type> struct lux_Pack<Type*> : lux_Name<Type*>
{
	std::ptrdiff_t size;
	Type *data;
	int ref;

	lux_Pack(Type *data=nullptr, std::ptrdiff_t size=0)
	{
		this->data = data;
		this->size = size;
		this->ref = LUA_NOREF;
	}
};

/// Storage class for const pointers inherits from raw pointers
template <class Type> struct lux_Pack<const Type*> : lux_Name<Type*>
{
	std::ptrdiff_t size;
	const Type *data;
	int ref;

	lux_Pack(const Type *data=nullptr, std::ptrdiff_t size=0)
	{
		this->data = data;
		this->size = size;
		this->ref = LUA_NOREF;
	}
};

/// Storage class for fixed size arrays inherits from pointers with caveats
template <class Type, std::size_t Size> struct lux_Pack<Type[Size]> : lux_Pack<Type*>
{
	lux_Pack(Type data[Size])
	{
		this->data = data;
		this->size = - Size; // Not owner -- note the sign
		this->ref = LUA_NOREF;
	}
};

/// Storage class for fixed size constant arrays inherits from constant points with caveats
template <class Type, std::size_t Size> struct lux_Pack<const Type[Size]> : lux_Pack<const Type *>
{
	lux_Pack(const Type data[Size])
	{
		this->data = data;
		this->size = - Size; // Not owner
		this->ref = LUA_NOREF;
	}
};

/// Basic stack operations for C user data (base case)
template <class Type> struct lux_Store : lux_Pack<Type>
{
	typedef lux_Store<Type> Store; // storage class

	/// Return storage if types match or not at all
	static Store *check(lua_State *state, int stack=1)
	{
		union {
		 void *address;
		 Store *user;
		};
		address = luaL_checkudata(state, stack, Store::name);
		return user;
	}

	/// Return storage if types match or nullptr
	static Store *test(lua_State *state, int stack=1)
	{
		union {
		 void *address;
		 Store *user;
		};
		address = luaL_testudata(state, stack, Store::name);
		return user;
	}

	/// Create storage for user data and put it on the stack
	static int push(lua_State *state, Type &&data, int size=0)
	{
		new (state) Store(data);
		luaL_setmetatable(state, Store::name);
		return 1;
	}

	/// Create storage for user data and put it on the stack
	static int push(lua_State *state, Type &data, int size=0)
	{
		new (state) Store(data);
		luaL_setmetatable(state, Store::name);
		return 1;
	}

	/// Create storage and user data
	static int push(lua_State *state)
	{
		new (state) Store;
		luaL_setmetatable(state, Store::name);
		return 1;
	}

	/// Return data if types match or the given default
	static Type &opt(lua_State *state, int stack, Type &opt)
	{
		auto user = test(state, stack);
		return user ? user->data : opt;
	}

	/// Return data if types match or not at all
	static Type &to(lua_State *state, int stack=1)
	{
		return check(state, stack)->data;
	}

	/// Inherit base constructor
	using lux_Pack<Type>::lux_Pack;
};

/// Specialized for pointers with conversion between nullptr/nil
template <class Type> struct lux_Store<Type*> : lux_Pack<Type*>
{
	typedef lux_Store<Type*> Store; // storage class

	/// Return storage if types match or not at all
	static Store *check(lua_State *state, int stack=1)
	{
		union {
		 void *address;
		 Store *user;
		};
		address = luaL_checkudata(state, stack, Store::name);
		return user;
	}

	/// Return storage if types match or nullptr
	static Store *test(lua_State *state, int stack=1)
	{
		union {
		 void *address;
		 Store *user;
		};
		address = luaL_testudata(state, stack, Store::name);
		return user;
	}

	/// Create storage for user data and put it on the stack
	static int push(lua_State *state, Type *data, int size=0)
	{
		if (data)
		{
			new (state) Store(data, size);
			luaL_setmetatable(state, Store::name);
		}
		else lua_pushnil(state);
		return 1;
	}

	/// Create storage and user data
	static int push(lua_State *state, int size=1)
	{
		auto data = new Type [size];
		auto user = new (state) Store(data, size);
		luaL_setmetatable(state, Store::name);
		return 1;
	}

	/// Return data if types match or the given default
	static Type *opt(lua_State *state, int stack=1, Type *opt=nullptr)
	{
		auto user = test(state, stack);
		return user ? user->data : opt;
	}

	/// Return data if types match or not at all
	static Type *to(lua_State *state, int stack=1)
	{
		if (lua_isnoneornil(state, stack)) return nullptr;
		return check(state, stack)->data;
	}

	/// Push with reference to owner where 'this' is on the stack
	int push(lua_State *state, Type *data, int size, int stack)
	{
		// Put data on the stack (not as owner)
		auto user = new (state) Store(data, - size);
		luaL_setmetatable(state, Store::name);
		// Not null and this is an array
		if (user and this->size not_eq 0)
		{
			// Begin pointer in range
			assert(this->data <= data);
			// End pointer is also in range
			assert(this->data + std::abs(this->size) >= data + size);
			// Store reference numbers in the metatable
			luaL_getmetatable(state, Store::name);
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
		else lua_pushnil(state);
		return 1;
	}

	/// Inherit base constructor
	using lux_Pack<Type*>::lux_Pack;
};

#endif // file

