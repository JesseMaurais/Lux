#ifndef __lxstore__
#define __lxstore__

#include "lxalloc.hpp"
#include <typeinfo>

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

	lux_Pack(User *data=nullptr, ssize_t size=0)
	{
		this->data = data;
		this->size = size;
	}
};

// The const pointer case

template <class User> struct lux_Pack<const User*> : lux_Pack<User*>
{
	lux_Pack(const User *data=nullptr, ssize_t size=0)
	{
		this->data = const_cast<User*>(data);
		this->size = size;
	}
};

// The fixed sized array case

template <class User, size_t Size> struct lux_Pack<User[Size]> : lux_Pack<User*>
{
	lux_Pack(User *data=nullptr)
	{
		this->data = data;
		this->size = -Size; // Not owner
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
		return new (state) Type(data, size);
	}

	static User &to(lua_State *state, int stack=1)
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
		if (data) return new (state) Type(data, size);
		lua_pushnil(state);
		return nullptr;
	}

	static User *to(lua_State *state, int stack=1)
	{
		if (lua_isnoneornil(state, stack)) return nullptr;
		return check(state, stack)->data;
	}

	using lux_Pack<User*>::lux_Pack;
};

#endif // file

