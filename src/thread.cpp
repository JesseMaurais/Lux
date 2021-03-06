/**
 * Support for pre-emptive multi-threading with mutual exclusion and condition
 * variables. It is based on the C++11 standard for threads so it will be more
 * portable than the POSIX threading module. However, it does not have as many
 * features and does not give the programmer as much control over the behavior
 * of generated threads, their attributes, or their locking mechanisms and the
 * attributes of these locks.
 *
 * This code does not currently catch exceptions properly and report them back
 * to the interpreter, so that this module may crash a terminal session if any
 * exception gets thrown. 
 */

#include "lux.hpp"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>


static lua_State *call(lua_State *state)
{
	// State contains only a function and its arguments
	lua_call(state, lua_gettop(state) - 1, LUA_MULTRET);
	return state;
}


struct Thread
{
	typedef std::thread User;
	typedef lux_Store<User> Type;

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
		// Explicit destruction
		Type::to(state).~thread();
		return 0;
	}

	static int __tostring(lua_State *state)
	{
		// Puts formatted string of metatable name and storage address
		lua_pushfstring(state, "%s: %p", Type::name, Type::test(state));
		return 1;
	}

	static int concurrency(lua_State *state)
	{
		// Number of hardware supported thread contexts
		size_t size = std::thread::hardware_concurrency();
		lua_pushinteger(state, size);
		return 1;
	}

	static int yield(lua_State *state)
	{
		// Always current thread
		std::this_thread::yield();
		return 0;
	}

	static int join(lua_State *state)
	{
		// Join with caller
		Type::to(state).join();
		return 0;
	}

	static int detach(lua_State *state)
	{
		// Make unjoinable
		Type::to(state).detach();
		return 0;
	}

	static int joinable(lua_State *state)
	{
		// Query attached/detached state
		bool joinable = Type::to(state).joinable();
		lua_pushboolean(state, joinable);
		return 1;
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

		lua_pushliteral(state, "concurrency");
		lua_pushcfunction(state, concurrency);
		lua_settable(state, -3);

		lua_pushliteral(state, "__gc");
		lua_pushcfunction(state, __gc);
		lua_settable(state, -3);

		lua_pushliteral(state, "__tostring");
		lua_pushcfunction(state, __tostring);
		lua_settable(state, -3);

		luaL_Reg index[] =
		{
		{"join", join},
		{"detach", detach},
		{"joinable", joinable},
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
	typedef std::mutex User;
	typedef lux_Store<User> Type;

	static int __new(lua_State *state)
	{
		// Default new
		Type::push(state);
		return 1;
	}

	static int __gc(lua_State *state)
	{
		using namespace std;
		// Explicit destruction
		Type::to(state).~mutex();
		return 0;
	}

	static int __tostring(lua_State *state)
	{
		// Puts formatted string of metatable name and storage address
		lua_pushfstring(state, "%s: %p", Type::name, Type::test(state));
		return 1;
	}

	static int lock(lua_State *state)
	{
		// Block until locked
		Type::to(state).lock();
		return 0;
	}

	static int unlock(lua_State *state)
	{
		// Unlock when locked
		Type::to(state).unlock();
		return 0;
	}

	static int trylock(lua_State *state)
	{
		// Do not block if a lock cannot be made
		bool locked = Type::to(state).try_lock();
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
		{"trylock", trylock},
		{nullptr}
		};

		lua_pushliteral(state, "__index");
		luaL_newlib(state, index);
		lua_settable(state, -3);

		return 1;
	}
};


struct Condition
{
	typedef std::condition_variable_any User;
	typedef lux_Store<User> Type;

	static int __new(lua_State *state)
	{
		// Default new
		Type::push(state);
		return 1;
	}

	static int __gc(lua_State *state)
	{
		using namespace std;
		// Explict destruction
		Type::to(state).~condition_variable_any();
		return 0;
	}

	static int __tostring(lua_State *state)
	{
		// Puts formatted string of metatable name and storage address
		lua_pushfstring(state, "%s: %p", Type::name, Type::test(state));
		return 1;
	}

	static int wait(lua_State *state)
	{
		auto &condition = Type::to(state, 1);
		auto &mutex = Mutex::Type::to(state, 2);
		// Block until notified
		condition.wait(mutex);
		return 0;
	}

	static int notify(lua_State *state)
	{
		// Resume one waiting thread
		Type::to(state).notify_one();
		return 0;
	}

	static int broadcast(lua_State *state)
	{
		// Resume all waiting threads
		Type::to(state).notify_all();
		return 0;
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
		{"wait", wait},
		{"notify", notify},
		{"broadcast", broadcast},
		{nullptr}
		};

		lua_pushliteral(state, "__index");
		luaL_newlib(state, index);
		lua_settable(state, -3);

		return 1;
	}
};


struct Future
{
	typedef std::future<lua_State*> User;
	typedef lux_Store<User> Type;

	static int __new(lua_State *state)
	{
		// Create independent execution stack
		lua_State *stack = lua_newthread(state);
		lua_pop(state, 1);
		// Move arguments to stack
		int top = lua_gettop(state);
		lua_xmove(state, stack, top);
		// Create with asynchronous execution
		Type::push(state, std::async(call, stack));
		return 1;
	}

	static int __gc(lua_State *state)
	{
		using namespace std;
		// Explicit destruction
		Type::to(state).~future();
		return 0;
	}

	static int __tostring(lua_State *state)
	{
		// Puts formatted string of metatable name and storage address
		lua_pushfstring(state, "%s: %p", Type::name, Type::test(state));
		return 1;
	}

	static int get(lua_State *state)
	{
		// Actually do 'get' on the future
		auto stack = Type::to(state).get();
		// Move results for return
		int top = lua_gettop(stack);
		lua_xmove(stack, state, top);
		return top;
	}

	static int wait(lua_State *state)
	{
		// Block until notified
		Type::to(state).wait();
		return 0;
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
		{"get", get},
		{"wait", wait},
		{nullptr}
		};

		lua_pushliteral(state, "__index");
		luaL_newlib(state, index);
		lua_settable(state, -3);

		return 1;
	}
};


struct Promise
{
	typedef lux_Store<std::promise<lua_State*>> Type;

	static int __new(lua_State *state)
	{
		// Default new
		Type::push(state);
		return 1;
	}

	static int __gc(lua_State *state)
	{
		using namespace std;
		// Explicit destruction
		Type::to(state).~promise();
		return 0;
	}

	static int __tostring(lua_State *state)
	{
		// Puts formatted string of metatable name and storage address
		lua_pushfstring(state, "%s: %p", Type::name, Type::test(state));
		return 1;
	}

	static int get(lua_State *state)
	{
		// Engage with the future using this promise
		Future::Type::push(state, Type::to(state).get_future());
		return 1;
	}

	static int set(lua_State *state)
	{
		auto &data = Type::to(state);
		// Create an independent execution stack
		lua_State *stack = lua_newthread(state);
		lua_remove(state, 1); // data
		lua_pop(state, 1); // stack
		// Move arguments to stack
		int top = lua_gettop(state);
		lua_xmove(state, stack, top);
		// Ready it in shared state
		data.set_value(stack);
		return 0;
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
		{"get", get},
		{"set", set},
		{nullptr}
		};

		lua_pushliteral(state, "__index");
		luaL_newlib(state, index);
		lua_settable(state, -3);

		return 1;
	}
};


// Lua module entry point

extern "C" int luaopen_thread(lua_State *state)
{
	if (Thread::open(state))
	{
		luaL_Reg regs[] =
		{
		{"mutex", Mutex::open},
		{"condition", Condition::open},
		{"future", Future::open},
		{"promise", Promise::open},
		{nullptr}
		};
		for (auto r=regs; r->name; ++r)
		{
		 luaL_requiref(state, r->name, r->func, false);
		 lua_setfield(state, -2, r->name);
		}
		return 1;
	}
	return 0;
}

