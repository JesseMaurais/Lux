#ifndef __lxtypes__
#define __lxtypes__

#include "lxalloc.hpp"
#include <typeinfo>

// The base case

template <class User> struct lux_Store
{
	User data;

	lux_Store(void) = default;

	lux_Store(User &data)
	{
		this->data = data;
	}

	static const char *name;
};

template <class User>
	const char *lux_Store<User>::name = typeid(User).name();


// The pointer case

template <class User> struct lux_Store<User*>
{
	User *data;
	size_t size;

	lux_Store(User *data=nullptr, size_t size=0)
	{
		this->data = data;
		this->size = size;
	}

	static const char *name;
};

template <class User>
	const char *lux_Store<User*>::name = typeid(User*).name();


// The const pointer case

template <class User> struct lux_Store<const User*>
{
	const User *data;
	size_t size;

	lux_Store(const User *data=nullptr, size_t size=0)
	{
		this->data = data;
		this->size = size;
	}

	static const char *name;
};

template <class User>
	const char *lux_Store<const User*>::name = lux_Store<User*>::name;


// The fixed sized array case

template <class User, int Size> struct lux_Store<User[Size]>
{
	User *data;
	size_t size;
	
	lux_Store(User *data=nullptr, size_t size=0)
	{
		this->data = data;
		this->size = 0; // Not owner
	}

	static const char *name;
};

template <class User, int Size>
	const char *lux_Store<User[Size]>::name = lux_Store<User*>::name;


// Basic stack operations for userdata

template <class User> struct lux_Type : lux_Store<User>
{
	typedef lux_Type<User> Type;
	using lux_Store<User>::lux_Store;

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

	static Type *push(lua_State *state, User data, size_t size=0)
	{
		return new (state) Type(data, size);
	}

	static User &to(lua_State *state, int stack=1)
	{
		return check(state, stack)->data;
	}
};

// Specialize for pointers -- convert between nullptr/nil

template <class User> struct lux_Type<User*> : lux_Store<User*>
{
	typedef lux_Type<User*> Type;
	using lux_Store<User*>::lux_Store;

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

	static Type *push(lua_State *state, User *data, size_t size=0)
	{
		if (data) return new (state) Type(data, size);
		lua_pushnil(state);
		return nullptr;
	}

	static User *to(lua_State *state, int stack=1)
	{
		if (lua_isnil(state, stack)) return nullptr;
		return check(state, stack)->data;
	}
};


#endif // file

