#include "lux.hpp"
#include <mutex>

typedef lux_Type<std::mutex> Type;

static int __new(lua_State *state)
{
	new (state) std::mutex;
	luaL_setmetatable(state, Type::name);
	return 1;
}

static int __gc(lua_State *state)
{
	using namespace std;
	Type *user = Type::check(state);
	user->data.~mutex();
	return 0;
}

static int __tostring(lua_State *state)
{
	lua_pushstring(state, Type::name);
	return 1;
}

static int lock(lua_State *state)
{
	Type *user = Type::check(state);
	user->data.lock();
	return 0;
}

static int unlock(lua_State *state)
{
	Type *user = Type::check(state);
	user->data.unlock();
	return 0;
}

static int try_lock(lua_State *state)
{
	Type *user = Type::check(state);
	bool locked = user->data.try_lock();
	lux_push(state, locked);
	return 1;
}

extern "C" int luaopen_mutex(lua_State *state)
{
	Type::name = lua_tostring(state, 1);
	luaL_newmetatable(state, Type::name);

	lua_pushliteral(state, "new");
	lua_pushcfunction(state, __new);
	lua_settable(state, -3);

	lua_pushliteral(state, "__gc");
	lua_pushcfunction(state, __gc);
	lua_settable(state, -3);

	lua_pushliteral(state, "__tostring");
	lua_pushcfunction(state, __tostring);
	lua_settable(state, -3);

	luaL_Reg index[] =
	{
	{"lock", lock},
	{"unlock", unlock},
	{"try_lock", try_lock},
	{nullptr}
	};

	lua_pushliteral(state, "__index");
	luaL_newlib(state, index);
	lua_settable(state, -3);

	return 1;
}

