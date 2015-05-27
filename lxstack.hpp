#ifndef __lxstack__
#define __lxstack__

#include "lxalloc.hpp"
#include <string>

int lux_stackdump(lua_State *state);

template <class C> struct lux_Type
{
	static const char *name;
};
template <class C> struct lux_User
{
	C data;
	size_t size;
};

template <class C> void lux_push(lua_State *state, C data, size_t size)
{
	typedef lux_Type<C> Type;
	typedef lux_User<C> User;
	auto user = new (state) User;
	luaL_setmetatable(state, Type::name);
	user->data = data;
	user->size = size;
}
template <class C> void lux_push(lua_State *state, C data)
{
	typedef lux_Type<C> Type;
	typedef lux_User<C> User;
	auto user = new (state) User;
	luaL_setmetatable(state, Type::name);
	user->data = data;
	user->size = 0;
}
template <class C> C lux_to(lua_State *state, int index)
{
	typedef lux_Type<C> Type;
	typedef lux_User<C> User;
	union {
	 User *user;
	 void *data;
	};
	data = luaL_checkudata(state, index, Type::name);
	return user->data;
}
template <class C> lux_User<C> *lux_check(lua_State *state, int index)
{
	typedef lux_Type<C> Type;
	typedef lux_User<C> User;
	union {
	 User *user;
	 void *data;
	};
	data = luaL_checkudata(state, index, Type::name);
	return user;
}
template <class C> lux_User<C> *lux_test(lua_State *state, int index)
{
	typedef lux_Type<C> Type;
	typedef lux_User<C> User;
	union {
	 User *user;
	 void *data;
	};
	data = luaL_testudata(state, index, Type::name);
	return user;
}

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

// strings
template <> void lux_push<std::string>(lua_State *state, std::string);
template <> std::string lux_to<std::string>(lua_State *state, int index);
template <> const char *lux_to<const char *>(lua_State *state, int index);


#endif // file

