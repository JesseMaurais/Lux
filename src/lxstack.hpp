#ifndef __lxstack__
#define __lxstack__

/**
 * For purposes of template programming, we want more indirect access to the
 * Lua stack operation functions. We need compile-time deduction of the type
 * of argument and for the C++ compiler to select the correct push/to at the
 * time it fills out the template. We also want in-place substitution of the
 * selected functions (that is, inline) since they do nothing more than hint
 * which Lua stack function to use. All these template push/opt/to functions
 * should be inlined away at compile time so that they do not imply any more
 * function call overhead. They should be thought of as rules which map from
 * C++ types to Lua types rather than usual functions. Any types not already
 * represented here can be implemented elsewhere and if they are seen by Lux
 * then they will also be substituted where appropriate.
 */

#include "lxstore.hpp"
#include <utility>
#include <tuple>

/// Variadic push -- for stacking many data at once
template <class User, class... Args> inline
int lux_push(lua_State *state, User data, Args... args)
{
	return lux_push(state, data) + lux_push(state, args...);
}

/// Generic push -- assume that argument is user data
template <class User> inline
int lux_push(lua_State *state, User data)
{
	typedef lux_Store<User> Type;
	return Type::push(state, data);
}

/// Generic opt -- user data as an optional argument
template <class User> inline
User lux_opt(lua_State *state, int stack, User opt)
{
	typedef lux_Store<User> Type;
	return Type::opt(state, stack, opt);
}

/// Generic to -- convert user data at given stack index
template <class User> inline
User lux_to(lua_State *state, int stack)
{
	typedef lux_Store<User> Type;
	return Type::to(state, stack);
}

// Full function template specialization of push/to for POD types

template <> inline
int lux_push<bool>(lua_State *state, bool value)
{
	lua_pushboolean(state, value);
	return 1;
}
template <> inline
int lux_push<int>(lua_State *state, int value)
{
	lua_pushinteger(state, value);
	return 1;
}
template <> inline
int lux_push<char>(lua_State *state, char value)
{
	lua_pushinteger(state, value);
	return 1;
}
template <> inline
int lux_push<short>(lua_State *state, short value)
{
	lua_pushinteger(state, value);
	return 1;
}
template <> inline
int lux_push<long>(lua_State *state, long value)
{
	lua_pushinteger(state, value);
	return 1;
}
template <> inline
int lux_push<float>(lua_State *state, float value)
{
	lua_pushnumber(state, value);
	return 1;
}
template <> inline
int lux_push<double>(lua_State *state, double value)
{
	lua_pushnumber(state, value);
	return 1;
}
template <> inline
int lux_push<void*>(lua_State *state, void *value)
{
	lua_pushlightuserdata(state, value);
	return 1;
}
template <> inline
int lux_push<long long>(lua_State *state, long long value)
{
	lua_pushinteger(state, value);
	return 1;
}
template <> inline
int lux_push<long double>(lua_State *state, long double value)
{
	lua_pushnumber(state, value);
	return 1;
}
template <> inline
int lux_push<lua_CFunction>(lua_State *state, lua_CFunction value)
{
	lua_pushcfunction(state, value);
	return 1;
}

// For unsigned types

template <> inline
int lux_push<unsigned int>(lua_State *state, unsigned int value)
{
	lua_pushinteger(state, value);
	return 1;
}
template <> inline
int lux_push<unsigned char>(lua_State *state, unsigned char value)
{
	lua_pushinteger(state, value);
	return 1;
}
template <> inline
int lux_push<unsigned short>(lua_State *state, unsigned short value)
{
	lua_pushinteger(state, value);
	return 1;
}
template <> inline
int lux_push<unsigned long>(lua_State *state, unsigned long value)
{
	lua_pushinteger(state, value);
	return 1;
}
template <> inline
int lux_push<unsigned long long>(lua_State *state, unsigned long long value)
{
	lua_pushinteger(state, value);
	return 1;
}

// Special case of explicit nullptr argument

template <> inline
int lux_push<std::nullptr_t>(lua_State *state, std::nullptr_t)
{
	lua_pushnil(state);
	return 1;
}

// Special case of C FILE handle

template <> inline
int lux_push<FILE*>(lua_State *state, FILE *value)
{
	auto stream = new (state) luaL_Stream;
	stream->f = value;
	stream->closef = nullptr;
	luaL_setmetatable(state, LUA_FILEHANDLE);
	return 1;
}

// Optional argument conversion forms

template <> inline
bool lux_opt<bool>(lua_State *state, int stack, bool opt)
{
	return luaL_optinteger(state, stack, opt);
}
template <> inline
int lux_opt<int>(lua_State *state, int stack, int opt)
{
	return luaL_optinteger(state, stack, opt);
}
template <> inline
char lux_opt<char>(lua_State *state, int stack, char opt)
{
	return luaL_optinteger(state, stack, opt);
}
template <> inline
short lux_opt<short>(lua_State *state, int stack, short opt)
{
	return luaL_optinteger(state, stack, opt);
}
template <> inline
long lux_opt<long>(lua_State *state, int stack, long opt)
{
	return luaL_optinteger(state, stack, opt);
}
template <> inline
float lux_opt<float>(lua_State *state, int stack, float opt)
{
	return luaL_optnumber(state, stack, opt);
}
template <> inline
double lux_opt<double>(lua_State *state, int stack, double opt)
{
	return luaL_optnumber(state, stack, opt);
}
template <> inline
void *lux_opt<void *>(lua_State *state, int stack, void *opt)
{
	void *address = lua_touserdata(state, stack);
	return address ? address : opt;
}
template <> inline
long long lux_opt<long long>(lua_State *state, int stack, long long opt)
{
	return luaL_optinteger(state, stack, opt);
}
template <> inline
long double lux_opt<long double>(lua_State *state, int stack, long double opt)
{
	return luaL_optnumber(state, stack, opt);
}
template <> inline
lua_CFunction lux_opt(lua_State *state, int stack, lua_CFunction opt)
{
	lua_CFunction function = lua_tocfunction(state, stack);
	return function ? function : opt;
}

// For unsigned types

template <> inline
unsigned int lux_opt(lua_State *state, int stack, unsigned int opt)
{
	return luaL_optinteger(state, stack, opt);
}
template <> inline
unsigned char lux_opt(lua_State *state, int stack, unsigned char opt)
{
	return luaL_optinteger(state, stack, opt);
}
template <> inline
unsigned short lux_opt(lua_State *state, int stack, unsigned short opt)
{
	return luaL_optinteger(state, stack, opt);
}
template <> inline
unsigned long lux_opt(lua_State *state, int stack, unsigned long opt)
{
	return luaL_optinteger(state, stack, opt);
}
template <> inline
unsigned long long lux_opt(lua_State *state, int stack, unsigned long long opt)
{
	return luaL_optinteger(state, stack, opt);
}

// Special case of C File handle

template <> inline
FILE *lux_opt<FILE *>(lua_State *state, int stack, FILE *opt)
{
	union {
	 luaL_Stream *stream;
	 void *address;
	};
	address = luaL_testudata(state, stack, LUA_FILEHANDLE);
	return address ? stream->f : opt;
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
	if (lua_isstring(state, stack))
	return *lua_tostring(state, stack);
	else
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
template <> inline
double lux_to<double>(lua_State *state, int stack)
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
	if (lua_isstring(state, stack))
	return *lua_tostring(state, stack);
	else
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
	if (lua_isstring(state, stack)) return lua_tostring(state, stack);
	return lux_Store<char*>::to(state, stack);
}

// Special case of C FILE handle

template <> inline
FILE *lux_to<FILE *>(lua_State *state, int stack)
{
	union {
	 luaL_Stream *stream;
	 void *address;
	};
	address = luaL_checkudata(state, stack, LUA_FILEHANDLE);
	return stream->f;
}

/* The following two 'push' functions are implemented quite differently than
 * those above, owing to the fact that C++ forbids partial function template
 * specialization. We get around this using partial specialization for class
 * templates, which is permitted, and having the default 'push' call members
 * of these classes. The default implementation for those storage classes is
 * found in lxstore.hpp, but we specialize them for pairs and tuples and get
 * push functions for these with any template arguments we want. Both tuples
 * and pairs push their contents on the stack, and we only implement push so
 * we get multiple return values in C++ correspond to multiple return values
 * in Lua, just as one would expect.
 */

// Partial specialization of storage class for std::pair
template <class First, class Second> struct lux_Store<std::pair<First, Second>>
{
	typedef std::pair<First, Second> User;
	typedef lux_Store<User> Type;

	static int push(lua_State *state, const User &data, int size=0)
	{
		// Call variadic push (top of this source file)
		return lux_push(state, data.first, data.second);
	}
};

// Partial specialization of storage class for std::tuple
template <class... Args> struct lux_Store<std::tuple<Args...>>
{
	typedef std::tuple<Args...> User;
	typedef lux_Store<User> Type;

	template <size_t... Index> static int push(lua_State *state, const User &data, std::index_sequence<Index...>)
	{
		// Call variadic push (top of this source file)
		return lux_push(state, std::get<Index>(data)...);
	}

	static int push(lua_State *state, const User &data, int size=0)
	{
		// Create an index sequence for the template parameter pack
		return push(state, data, std::index_sequence_for<Args...>{});
	}
};

#endif // file

