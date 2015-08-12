/**
 * Support for pre-emptive multi-threading with mutual exclusion. Based on the
 * C++11 standard for threads. It's somewhat incomplete since it does not also
 * have support for condition variables and thread local storage.
 */

#include "lux.hpp"
#include <thread>
#include <mutex>

struct Thread
{
	typedef lux_Store<std::thread> Type;

	static void call(lua_State *state)
	{
		lua_call(state, lua_gettop(state) - 1, 0);
	}

	static int __new(lua_State *state)
	{
		// Create independent execution stack
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

	static int open(lua_State *state)
	{
		Type::name = lua_tostring(state, 1);
		luaL_newmetatable(state, Type::name);

		lua_pushliteral(state, "new");
		lua_pushcfunction(state, __new);
		lua_settable(state, -3);

		lua_pushliteral(state, "yield");
		lua_pushcfunction(state, yield);
		lua_settable(state, -3);

		lua_pushliteral(state, "__gc");
		lua_pushcfunction(state, __gc);
		lua_settable(state, -3);

		lua_pushliteral(state, "__tostring");
		lua_pushcfunction(state, __tostring);
		lua_settable(state, -3);

		luaL_Reg index[] =
		{
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
};

struct Mutex
{
	typedef lux_Store<std::mutex> Type;

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

	static int open(lua_State *state)
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
};


extern "C" int luaopen_thread(lua_State *state)
{
	luaL_Reg regs[] =
	{
	{"thread", Thread::open},
	{"mutex", Mutex::open},
	{nullptr}
	};
	for (auto r=regs; r->name; ++r)
	{
	 luaL_requiref(state, r->name, r->func, true);
	 lua_pop(state, 1);
	}
	return 0;
}

