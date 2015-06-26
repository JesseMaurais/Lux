#ifndef __lxstack__
#define __lxstack__

#include "lxalloc.hpp"
#include <typeinfo>

// print entire stack to stdout
int lux_stackdump(lua_State *state);

// interface for full userdata
template <class C> struct lux_Type
{
	C data;
	size_t size;
	static const char *name;

	lux_Type(C data, size_t size=0)
	{
		this->data = data;
		this->size = size;
	}
};

// ensures emission of type but string format is implementation dependent
template <class C> const char *lux_Type<C>::name = typeid(C).name();

// be invariant to const-ness of pointers
template <class C> struct lux_Type<const C*>
{
	const C *data;
	size_t size;
	static const char *name;

	lux_Type(const C *data, size_t size=0)
	{
		this->data = data;
		this->size = size;
	}
};

// use the same metatable as the class without the const qualifier
template <class C> const char *lux_Type<const C*>::name = lux_Type<C*>::name;

// treat arrays as if they were pointers
template <class C, int n> struct lux_Type<C[n]>
{
	C *data;
	size_t size;
	static const char *name;
	lux_Type(C *data, size_t size=0)
	{
		this->data = data;
		this->size = size;
	}
};

// use the same metatable as the class defined as pointer rather than array
template <class C, int n> const char *lux_Type<C[n]>::name = lux_Type<C*>::name;


// register a metatable for a type
template <class C> bool lux_newtype(lua_State *state, const char *name=nullptr)
{
	typedef lux_Type<C> Type;
	if (name) Type::name = name;
	return luaL_newmetatable(state, Type::name);
}

// generic push for opaque userdata types with storage size
template <class C> void lux_push(lua_State *state, C data, size_t size)
{
	typedef lux_Type<C> Type;
	if (!data)
	{
	 lua_pushnil(state);
	 return;
	}
	auto user = new (state) Type(data, size);
	luaL_setmetatable(state, Type::name);
}

// generic push for opaque userdata types (general form)
template <class C> void lux_push(lua_State *state, C data)
{
	typedef lux_Type<C> Type;
	if (!data)
	{
	 lua_pushnil(state);
	 return;
	}
	auto user = new (state) Type(data);
	luaL_setmetatable(state, Type::name);
}

// generic to -- userdata from the stack at given index
template <class C> C lux_to(lua_State *state, int index)
{
	typedef lux_Type<C> Type;
	union {
	 Type *user;
	 void *data;
	};
	data = luaL_checkudata(state, index, Type::name);
	return user->data;
}

// obtain the box pointer rather than the userdata
template <class C> lux_Type<C> *lux_check(lua_State *state, int index)
{
	typedef lux_Type<C> Type;
	union {
	 Type *user;
	 void *data;
	};
	data = luaL_checkudata(state, index, Type::name);
	return user;
}

// obtain the box pointer but return null if there's a type mismatch
template <class C> lux_Type<C> *lux_test(lua_State *state, int index)
{
	typedef lux_Type<C> Type;
	union {
	 Type *user;
	 void *data;
	};
	data = luaL_testudata(state, index, Type::name);
	return user;
}

// full function template specialization of push/to for POD types

template <> void lux_push<bool>(lua_State *state, bool);
template <> void lux_push<int>(lua_State *state, int);
template <> void lux_push<char>(lua_State *state, char);
template <> void lux_push<short>(lua_State *state, short);
template <> void lux_push<long>(lua_State *state, long);
template <> void lux_push<float>(lua_State *state, float);
template <> void lux_push<double>(lua_State *state, double);
template <> void lux_push<void*>(lua_State *state, void*);

template <> void lux_push<unsigned int>(lua_State *state, unsigned int);
template <> void lux_push<unsigned char>(lua_State *state, unsigned char);
template <> void lux_push<unsigned short>(lua_State *state, unsigned short);
template <> void lux_push<unsigned long>(lua_State *state, unsigned long);

template <> bool lux_to<bool>(lua_State *state, int index);
template <> int lux_to<int>(lua_State *state, int index);
template <> char lux_to<char>(lua_State *state, int index);
template <> short lux_to<short>(lua_State *state, int index);
template <> long lux_to<long>(lua_State *state, int index);
template <> float lux_to<float>(lua_State *state, int index);
template <> double lux_to<double>(lua_State *state, int index);
template <> void *lux_to<void*>(lua_State *state, int index);

template <> unsigned int lux_to<unsigned int>(lua_State *state, int index);
template <> unsigned char lux_to<unsigned char>(lua_State *state, int index);
template <> unsigned short lux_to<unsigned short>(lua_State *state, int index);
template <> unsigned long lux_to<unsigned long>(lua_State *state, int index);

template <> const char *lux_to<const char *>(lua_State *state, int index);

#endif // file

