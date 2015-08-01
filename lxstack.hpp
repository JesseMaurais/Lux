#ifndef __lxstack__
#define __lxstack__

#include "lxstore.hpp"

// Print entire stack to stdout

int lux_stackdump(lua_State *state);

// Variadic push -- for stacking many data at once

template <class User, class... Args> inline
int lux_push(lua_State *state, User data, Args... args)
{
	lux_push(state, data);
	lux_push(state, args...);
	return sizeof...(Args) + 1;
}

// Generic push -- assume that argument is a userdata
 
template <class User> inline
void lux_push(lua_State *state, User data)
{
	typedef lux_Store<User> Type;
	(void) Type::push(state, data);
	luaL_setmetatable(state, Type::name);
}

// Generic to -- convert userdata at given stack index

template <class User> inline
User lux_to(lua_State *state, int stack)
{
	typedef lux_Store<User> Type;
	return Type::to(state, stack);
}

// Full function template specialization of push/to for POD types

template <> inline
void lux_push<bool>(lua_State *state, bool value)
{
	lua_pushboolean(state, value);
}
template <> inline
void lux_push<int>(lua_State *state, int value)
{
	lua_pushinteger(state, value);
}
template <> inline
void lux_push<char>(lua_State *state, char value)
{
	lua_pushinteger(state, value);
}
template <> inline
void lux_push<short>(lua_State *state, short value)
{
	lua_pushinteger(state, value);
}
template <> inline
void lux_push<long>(lua_State *state, long value)
{
	lua_pushinteger(state, value);
}
template <> inline
void lux_push<float>(lua_State *state, float value)
{
	lua_pushnumber(state, value);
}
template <> inline
void lux_push<double>(lua_State *state, double value)
{
	lua_pushnumber(state, value);
}
template <> inline
void lux_push<void*>(lua_State *state, void *value)
{
	lua_pushlightuserdata(state, value);
}
template <> inline
void lux_push<long long>(lua_State *state, long long value)
{
	lua_pushinteger(state, value);
}
template <> inline
void lux_push<long double>(lua_State *state, long double value)
{
	lua_pushnumber(state, value);
}
template <> inline
void lux_push<lua_CFunction>(lua_State *state, lua_CFunction value)
{
	lua_pushcfunction(state, value);
}

// For unsigned types

template <> inline
void lux_push<unsigned int>(lua_State *state, unsigned int value)
{
	lua_pushinteger(state, value);
}
template <> inline
void lux_push<unsigned char>(lua_State *state, unsigned char value)
{
	lua_pushinteger(state, value);
}
template <> inline
void lux_push<unsigned short>(lua_State *state, unsigned short value)
{
	lua_pushinteger(state, value);
}
template <> inline
void lux_push<unsigned long>(lua_State *state, unsigned long value)
{
	lua_pushinteger(state, value);
}
template <> inline
void lux_push<unsigned long long>(lua_State *state, unsigned long long value)
{
	lua_pushinteger(state, value);
}

// Special case of explicit nullptr argument

template <> inline
void lux_push<std::nullptr_t>(lua_State *state, std::nullptr_t)
{
	lua_pushnil(state);
}

// Special case of C FILE handle

template <> inline
void lux_push<FILE*>(lua_State *state, FILE *value)
{
	auto stream = new (state) luaL_Stream;
	stream->f = value;
	stream->closef = nullptr;
	luaL_setmetatable(state, LUA_FILEHANDLE);
}

// "to" rather than "check" counterparts in stack conversion

template <> inline
bool lux_to<bool>(lua_State *state, int stack)
{
	return lua_toboolean(state, stack);
}
template <> inline
int lux_to<int>(lua_State *state, int stack)
{
	return lua_tointeger(state, stack);
}
template <> inline
char lux_to<char>(lua_State *state, int stack)
{
	return lua_tointeger(state, stack);
}
template <> inline
short lux_to<short>(lua_State *state, int stack)
{
	return lua_tointeger(state, stack);
}
template <> inline
long lux_to<long>(lua_State *state, int stack)
{
	return lua_tointeger(state, stack);
}
template <> inline
float lux_to<float>(lua_State *state, int stack)
{
	return lua_tonumber(state, stack);
}
template <> double lux_to<double>(lua_State *state, int stack)
{
	return lua_tonumber(state, stack);
}
template <> inline
void *lux_to<void*>(lua_State *state, int stack)
{
	return lua_touserdata(state, stack);
}
template <> inline
long long lux_to<long long>(lua_State *state, int stack)
{
	return lua_tointeger(state, stack);
}
template <> inline
long double lux_to<long double>(lua_State *state, int stack)
{
	return lua_tonumber(state, stack);
}
template <> inline
lua_CFunction lux_to<lua_CFunction>(lua_State *state, int stack)
{
	return lua_tocfunction(state, stack);
}

// For unsigned types

template <> inline
unsigned int lux_to<unsigned int>(lua_State *state, int stack)
{
	return lua_tointeger(state, stack);
}
template <> inline
unsigned char lux_to<unsigned char>(lua_State *state, int stack)
{
	return lua_tointeger(state, stack);
}
template <> inline
unsigned short lux_to<unsigned short>(lua_State *state, int stack)
{
	return lua_tointeger(state, stack);
}
template <> inline
unsigned long lux_to<unsigned long>(lua_State *state, int stack)
{
	return lua_tointeger(state, stack);
}
template <> inline
unsigned long long lux_to<unsigned long long>(lua_State *state, int stack)
{
	return lua_tointeger(state, stack);
}

// Special case of string representation in char array

template <> inline
const char *lux_to<const char *>(lua_State *state, int stack)
{
	if (lua_isstring(state, stack))
	{
		return lua_tostring(state, stack);
	}
	typedef lux_Store<char*> Type;
	return Type::to(state, stack);
}

// Special case of Lua file stream

template <> inline
luaL_Stream *lux_to<luaL_Stream*>(lua_State *state, int stack)
{
	union {
	 luaL_Stream *stream;
	 void *address;
	};
	address = luaL_checkudata(state, stack, LUA_FILEHANDLE);
	return stream;
}

// Special case of C FILE stream

template <> inline
FILE *lux_to<FILE*>(lua_State *state, int stack)
{
	return lux_to<luaL_Stream*>(state, stack)->f;
}


#endif // file

