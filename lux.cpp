#include "lux.hpp"


// LXALLOC.HPP ////////////////////////////////////////////////////////////////


void *operator new (size_t size, lua_State *state)
{
	return lua_newuserdata(state, size);
}

void *operator new [] (size_t size, lua_State *state)
{
	return lua_newuserdata(state, size);
}


// LXARRAY.HPP ////////////////////////////////////////////////////////////////


template <> const char *lux_Array<int>::Type::name = "int";
template <> const char *lux_Array<char>::Type::name = "char";
template <> const char *lux_Array<short>::Type::name = "short";
template <> const char *lux_Array<long>::Type::name = "long";
template <> const char *lux_Array<float>::Type::name = "float";
template <> const char *lux_Array<double>::Type::name = "double";

template <> const char *lux_Array<unsigned int>::Type::name = "uint";
template <> const char *lux_Array<unsigned char>::Type::name = "uchar";
template <> const char *lux_Array<unsigned short>::Type::name = "ushort";
template <> const char *lux_Array<unsigned long>::Type::name = "ulong";

int luxopen_array(lua_State *state)
{
	lux_Array<int>::open(state);
	lux_Array<char>::open(state);
	lux_Array<short>::open(state);
	lux_Array<long>::open(state);
	lux_Array<float>::open(state);
	lux_Array<double>::open(state);

	lux_Array<unsigned int>::open(state);
	lux_Array<unsigned char>::open(state);
	lux_Array<unsigned short>::open(state);
	lux_Array<unsigned long>::open(state);

	return 0;
}

template <> const char *lux_Type<const char*>::name = lux_Array<char>::Type::name;


// LXSTACK.HPP ////////////////////////////////////////////////////////////////


int lux_stackdump(lua_State *state)
{
	int top = lua_gettop(state);
	for (int it = 1; it <= top; ++it)
	{
	 const char *name = luaL_typename(state, it);
	 switch (lua_type(state, it))
	 {
	   case LUA_TNIL:
	   case LUA_TNONE:
	   case LUA_TFUNCTION:
		printf("[%d] %s\n", it, name);
		break;
	   case LUA_TNUMBER:
		printf("[%d] %s: %f\n", it, name, lua_tonumber(state, it));
		break;
	   case LUA_TSTRING:
		printf("[%d] %s: %s\n", it, name, lua_tostring(state, it));
		break;
	   case LUA_TTHREAD:
		printf("[%d] %s: %p\n", it, name, lua_tothread(state, it));
		break;
	   case LUA_TBOOLEAN:
		printf("[%d] %s: %d\n", it, name, lua_toboolean(state, it));
		break;
	   case LUA_TLIGHTUSERDATA:
		printf("[%d] %s: %p\n", it, name, lua_touserdata(state, it));
		break;
	   case LUA_TTABLE:
	   case LUA_TUSERDATA:
		if (luaL_callmeta(state, it, "__tostring"))
		{
		 name = lua_tostring(state, -1);
		 printf("[%d] %s\n", it, name);
		 lua_pop(state, 1);
		}
		else
		 printf("[%d] %s: %p\n", it, name, lua_topointer(state, it));
		break;
	 }
	}
	return 0;
}


template <> void lux_push<bool>(lua_State *state, bool value)
{
	lua_pushboolean(state, value);
}
template <> void lux_push<int>(lua_State *state, int value)
{
	lua_pushinteger(state, value);
}
template <> void lux_push<char>(lua_State *state, char value)
{
	lua_pushinteger(state, value);
}
template <> void lux_push<short>(lua_State *state, short value)
{
	lua_pushinteger(state, value);
}
template <> void lux_push<long>(lua_State *state, long value)
{
	lua_pushinteger(state, value);
}
template <> void lux_push<float>(lua_State *state, float value)
{
	lua_pushnumber(state, value);
}
template <> void lux_push<double>(lua_State *state, double value)
{
	lua_pushnumber(state, value);
}
template <> void lux_push<void*>(lua_State *state, void *value)
{
	lua_pushlightuserdata(state, value);
}
template <> void lux_push<std::string>(lua_State *state, std::string value)
{
	lua_pushstring(state, value.c_str());
}


template <> void lux_push<unsigned int>(lua_State *state, unsigned int value)
{
	lua_pushunsigned(state, value);
}
template <> void lux_push<unsigned char>(lua_State *state, unsigned char value)
{
	lua_pushunsigned(state, value);
}
template <> void lux_push<unsigned short>(lua_State *state, unsigned short value)
{
	lua_pushunsigned(state, value);
}
template <> void lux_push<unsigned long>(lua_State *state, unsigned long value)
{
	lua_pushunsigned(state, value);
}


template <> bool lux_to<bool>(lua_State *state, int index)
{
	return lua_toboolean(state, index);
}
template <> int lux_to<int>(lua_State *state, int index)
{
	return lua_tointeger(state, index);
}
template <> char lux_to<char>(lua_State *state, int index)
{
	return lua_tointeger(state, index);
}
template <> short lux_to<short>(lua_State *state, int index)
{
	return lua_tointeger(state, index);
}
template <> long lux_to<long>(lua_State *state, int index)
{
	return lua_tointeger(state, index);
}
template <> float lux_to<float>(lua_State *state, int index)
{
	return lua_tonumber(state, index);
}
template <> double lux_to<double>(lua_State *state, int index)
{
	return lua_tonumber(state, index);
}
template <> void *lux_to<void*>(lua_State *state, int index)
{
	return lua_touserdata(state, index);
}
template <> std::string lux_to<std::string>(lua_State *state, int index)
{
	return lua_tostring(state, index);
}


template <> unsigned int lux_to<unsigned int>(lua_State *state, int index)
{
	return lua_tounsigned(state, index);
}
template <> unsigned char lux_to<unsigned char>(lua_State *state, int index)
{
	return lua_tounsigned(state, index);
}
template <> unsigned short lux_to<unsigned short>(lua_State *state, int index)
{
	return lua_tounsigned(state, index);
}
template <> unsigned long lux_to<unsigned long>(lua_State *state, int index)
{
	return lua_tounsigned(state, index);
}


template <> const char *lux_to<const char *>(lua_State *state, int index)
{
	if (lua_isstring(state, index)) return lua_tostring(state, index);
	auto user = lux_test<char*>(state, index);
	return user ? user->data : nullptr;
}

