#include "lux.hpp"
#include <thread>

typedef lux_Type<std::thread> Type;

static void call(lua_State *state)
{
	lua_call(state, lua_gettop(state)-1, 0);
}

static int __new(lua_State *state)
{
	// Create an independent execution stack
	lua_State *stack = lua_newthread(state);
	lua_pop(state, 1);
	// Copy the stack contents
	int top = lua_gettop(state);
	lua_xmove(state, stack, top);
	// Launch thread and return a handle
	new (state) std::thread(call, stack);
	luaL_setmetatable(state, Type::name);
	return 1;
}

static int __gc(lua_State *state)
{
	using namespace std;
	Type *user = Type::check(state);
	user->data.~thread();
	return 0;
}

static int __tostring(lua_State *state)
{
	lua_pushstring(state, Type::name);
	return 1;
}

static int yield(lua_State *state)
{
	std::this_thread::yield();
	return 0;
}

static int detach(lua_State *state)
{
	Type *user = Type::check(state);
	user->data.detach();
	return 0;
}

static int joinable(lua_State *state)
{
	Type *user = Type::check(state);
	bool joinable = user->data.joinable();
	lux_push(state, joinable);
	return 1;
}

static int join(lua_State *state)
{
	Type *user = Type::check(state);
	user->data.join();
	return 0;
}

static int swap(lua_State *state)
{
	Type *user = Type::check(state, 1);
	Type *that = Type::check(state, 2);
	user->data.swap(that->data);
	return 0;
}

extern "C" int luaopen_thread(lua_State *state)
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
	{"yield", yield},
	{"detach", detach},
	{"joinable", joinable},
	{"join", join},
	{"swap", swap},
	{nullptr}
	};

	lua_pushliteral(state, "__index");
	luaL_newlib(state, index);
	lua_settable(state, -3);

	return 1;
}	

