#include "lux.hpp"
#include <pthread.h>
#include <cstdlib>


// Storage class for POSIX types
template <class User> struct Store : lux_Store<User>
{
	using Type = lux_Store<User>;

	// Called when module is loaded
	static int open(lua_State *state)
	{
		Type::name = lua_tostring(state, 1);
		if (luaL_newmetatable(state, Type::name))
		{
			lua_pushliteral(state, "init");
			lua_pushcfunction(state, __new);
			lua_settable(state, -3);

			lua_pushliteral(state, "__gc");
			lua_pushcfunction(state, __gc);
			lua_settable(state, -3);

			lua_pushliteral(state, "__tostring");
			lua_pushcfunction(state, __tostring);
			lua_settable(state, -3);

			lua_pushliteral(state, "__index");
			luaL_newlib(state, index);
			lua_settable(state, -3);
		}
		return 1;
	}

	// Class initialization routine
	static int init(lua_State *state, User &data);

	// Class cleanup routine
	static int destroy(lua_State *state, User &data);

	// Object allocation function
	static int __new(lua_State *state)
	{
		auto user = new (state) Type;
		luaL_setmetatable(state, Type::name);
		return init(state, user->data);
	}

	// Garbage collection metamethod
	static int __gc(lua_State *state)
	{
		auto data = Type::to(state);
		return destroy(state, data);
	}

	// String conversion is common to all
	static int __tostring(lua_State *state)
	{
		auto data = Type::to(state);
		lua_pushfstring(state, "%s: %p", Type::name, &data);
		return 1;
	}

	// Class methods index
	static luaL_Reg index[];
};


/** Threads ******************************************************************/


// POSIX native thread function
static void *start(void *stack)
{
	// Convert thread argument to a state
	lua_State *state = (lua_State*) stack;
	// Assume that a function and arguments are on it
	lua_call(state, lua_gettop(state) - 1, LUA_MULTRET);
	// Return the state
	pthread_exit(stack);
	pthread_testcancel();
	pthread_exit(nullptr);
}

// Create and start a new thread
static int create(lua_State *state)
{
	pthread_t id;
	// Create an independent execution stack
	lua_State *stack = lua_newthread(state);
	lua_pop(state, 1);
	// Copy the stack contents over to thread
	lua_xmove(state, stack, lua_gettop(state));
	// Launch thread but long jump if an error occurs
	int error = pthread_create(&id, nullptr, start, stack);
	if (error) return lux_perror(state, error);
	// Return thread id
	lux_push(state, id);
	return 1;
}

// Detach calling thread from main
static int detach(lua_State *state)
{
	auto id = lux_to<pthread_t>(state, 1);
	int error = pthread_detach(id);
	if (error) return lux_perror(state, error);
	else return 0;
}

// Test that two ids are same thread
static int equal(lua_State *state)
{
	auto first = lux_to<pthread_t>(state, 1);
	auto second = lux_to<pthread_t>(state, 2);
	lua_pushboolean(state, pthread_equal(first, second));
	return 1;
}

// Wait on thread and return values
static int join(lua_State *state)
{
	lua_State *stack;
	auto id = lux_to<pthread_t>(state, 1);
	// Join, get the execution stack it returns
	int error = pthread_join(id, (void**) &stack);
	if (error) return lux_perror(state, error);
	if (stack) // thread not cancelled
	{
		// Move thread results here
		int top = lua_gettop(stack);
		lua_xmove(stack, state, top);
		return top;
	}
	return 0;
}

// Get id of the calling thread
static int self(lua_State *state)
{
	auto id = pthread_self();
	lux_push(state, id);
	return 1;
}

// Cancels execution of a thread
static int cancel(lua_State *state)
{
	auto id = lux_to<pthread_t>(state, 1);
	int error = pthread_cancel(id);
	if (error) return lux_perror(state, error);
	else return 0;
}

// Disable or enable cancelling of a thread
static int setcancelstate(lua_State *state)
{
	// Single argument must be one of these two strings
	const char *options[] = {"disabled", "enabled", nullptr};
	int opt = luaL_checkoption(state, 1, *options, options);
	// Convert the option to one of these POSIX constants
	opt = opt ? PTHREAD_CANCEL_ENABLE : PTHREAD_CANCEL_DISABLE;
	// Set state and get the former cancel state
	int error = pthread_setcancelstate(opt, &opt);
	if (error) return lux_perror(state, error);
	switch (opt) // Convert to string
	{
	case PTHREAD_CANCEL_DISABLE:
		lua_pushstring(state, options[0]);
		break;
	case PTHREAD_CANCEL_ENABLE:
		lua_pushstring(state, options[1]);
		break;
	}
	return 1;
}

// Change the way a thread is cancelled
static int setcanceltype(lua_State *state)
{
	// Single argument must be one of these two strings
	const char *options[] = {"deferred", "asynchronous", nullptr};
	int opt = luaL_checkoption(state, 1, *options, options);
	// Convert the string to one of these POSIX constants
	opt = opt ? PTHREAD_CANCEL_ASYNCHRONOUS : PTHREAD_CANCEL_DEFERRED;
	// Set type and get the former cancel type
	int error = pthread_setcanceltype(opt, &opt);
	if (error) return lux_perror(state, error);
	switch (opt) // Convert to string
	{
	case PTHREAD_CANCEL_ASYNCHRONOUS:
		lua_pushstring(state, options[1]);
		break;
	case PTHREAD_CANCEL_DEFERRED:
		lua_pushstring(state, options[0]);
		break;
	}
	return 1;
}

// Attempt to set the concurrency level
static int setconcurrency(lua_State *state)
{
	int level = lua_tointeger(state, 1);
	int error = pthread_setconcurrency(level);
	if (error) return lux_perror(state, error);
	return 0;
}

// Aquire the current concurrency level
static int getconcurrency(lua_State *state)
{
	int level = pthread_getconcurrency();
	lua_pushinteger(state, level);
	return 1;
}

// Attempt to set the scheduling priority
static int setschedprio(lua_State *state)
{
	auto id = lux_to<pthread_t>(state, 1);
	int prio = lua_tointeger(state, 2);
	int error = pthread_setschedprio(id, prio);
	if (error) return lux_perror(state, error);
	return 0;
}


/** Thread Attributes ********************************************************/


typedef Store<pthread_attr_t> pthread_attr; // Storage class for Lux

// Initialize the thread attributes to default
template <> int pthread_attr::init(lua_State *state, pthread_attr_t &attr)
{
	int error = pthread_attr_init(&attr);
	if (error) return lux_perror(state, error);
	return 1;
}

// Destroy attributes object on collection
template <> int pthread_attr::destroy(lua_State *state, pthread_attr_t &attr)
{
	int error = pthread_attr_destroy(&attr);
	if (error) return lux_perror(state, error);
	return 0;
}

// Create a thread with attributes
static int attr_create(lua_State *state)
{
	pthread_t thread;
	auto attr = pthread_attr::to(state);
	lua_remove(state, 1);
	// Create an independent execution stack
	lua_State *stack = lua_newthread(state);
	lua_pop(state, 1);
	// Copy the stack contents over to thread
	lua_xmove(state, stack, lua_gettop(state));
	// Launch thread but long jump if an error occurs
	int error = pthread_create(&thread, &attr, start, stack);
	if (error) return lux_perror(state, error);
	// Return the thread id
	lux_push(state, thread);
	return 1;
}

// Set the detach state of new threads
static int setdetachstate(lua_State *state)
{
	auto attr = pthread_attr::to(state, 1);
	// Single argument must be one of these two strings
	const char *opts[] = {"detached", "joinable", nullptr};
	int opt = luaL_checkoption(state, 2, *opts, opts);
	// Convert the option to one of these POSIX constants
	opt = opt ? PTHREAD_CREATE_JOINABLE : PTHREAD_CREATE_DETACHED;
	int error = pthread_attr_setdetachstate(&attr, opt);
	if (error) return lux_perror(state, error);
	return 0;
}

// Get the detach state of new threads
static int getdetachstate(lua_State *state)
{
	auto attr = pthread_attr::to(state);
	int opt, error = pthread_attr_getdetachstate(&attr, &opt);
	switch (opt) // Convert to string
	{
	case PTHREAD_CREATE_DETACHED:
		lua_pushliteral(state, "detached");
		break;
	case PTHREAD_CREATE_JOINABLE:
		lua_pushliteral(state, "joinable");
		break;
	}
	return 1;
}

// Set the guard size of new threads
static int setguardsize(lua_State *state)
{
	auto attr = pthread_attr::to(state, 1);
	size_t size = lua_tointeger(state, 2);
	int error = pthread_attr_setguardsize(&attr, size);
	if (error) return lux_perror(state, error);
	return 0;
}

// Get the guard size of new threads
static int getguardsize(lua_State *state)
{
	size_t size;
	auto attr = pthread_attr::to(state, 1);
	int error = pthread_attr_getguardsize(&attr, &size);
	if (error) return lux_perror(state, error);
	lua_pushinteger(state, size);
	return 1;
}

// Set scheduling inheritance scheme
static int setinheritsched(lua_State *state)
{
	auto attr = pthread_attr::to(state, 1);
	// Single argument must be one of these two strings
	const char *opts[] = {"inherit", "explicit", nullptr};
	int opt = luaL_checkoption(state, 2, *opts, opts);
	// Convert the option to one of these POSIX constants
	opt = opt ? PTHREAD_EXPLICIT_SCHED : PTHREAD_INHERIT_SCHED;
	int error = pthread_attr_setinheritsched(&attr, opt);
	if (error) return lux_perror(state, error);
	return 0;
}

// Get scheduling inheritance scheme
static int getinheritsched(lua_State *state)
{
	auto attr = pthread_attr::to(state, 1);
	int opt, error = pthread_attr_getinheritsched(&attr, &opt);
	if (error) return lux_perror(state, error);
	switch (opt) // Convert to string
	{
	case PTHREAD_INHERIT_SCHED:
		lua_pushliteral(state, "inherit");
		break;
	case PTHREAD_EXPLICIT_SCHED:
		lua_pushliteral(state, "explicit");
		break;
	}
	return 0;
}

// Set the contention scope for new threads
static int setscope(lua_State *state)
{
	auto attr = pthread_attr::to(state, 1);
	// Single argument must be one of these strings
	const char *opts[] = {"system", "process", nullptr};
	int opt = luaL_checkoption(state, 2, *opts, opts);
	// Convert the option to one of these POSIX constants
	opt = opt ? PTHREAD_SCOPE_PROCESS : PTHREAD_SCOPE_SYSTEM;
	int error = pthread_attr_setscope(&attr, opt);
	if (error) return lux_perror(state, error);
	return 0;
}

// Get the contention scope for new threads
static int getscope(lua_State *state)
{
	auto attr = pthread_attr::to(state, 1);
	int opt, error = pthread_attr_getscope(&attr, &opt);
	if (error) return lux_perror(state, error);
	switch (opt) // Convert to string
	{
	case PTHREAD_SCOPE_SYSTEM:
		lua_pushliteral(state, "system");
		break;
	case PTHREAD_SCOPE_PROCESS:
		lua_pushliteral(state, "process");
		break;
	}
	return 1;
}

// Set (true) execution stack for new threads
static int setstack(lua_State *state)
{
	auto attr = pthread_attr::to(state, 1);
	void *addr = lua_touserdata(state, 2);
	size_t size = lua_tointeger(state, 3);
	int error = pthread_attr_setstack(&attr, addr, size);
	if (error) return lux_perror(state, error);
	return 0;
}

// Get (true) execution stack for new threads
static int getstack(lua_State *state)
{
	size_t size;
	void *addr;
	auto attr = pthread_attr::to(state, 1);
	int error = pthread_attr_getstack(&attr, &addr, &size);
	if (error) return lux_perror(state, error);
	lua_pushlightuserdata(state, addr);
	lua_pushinteger(state, size);
	return 2;
}

/* DEPRECATED *****************************************************************
// Set (true) execution stack for new threads
static int setstackaddr(lua_State *state)
{
	auto attr = pthread_attr::to(state, 1);
	void *addr = lua_touserdata(state, 2);
	int error = pthread_attr_setstackaddr(&attr, addr);
	if (error) return lux_perror(state, error);
	return 0;
}

// Get (true) execution stack for new threads
static int getstackaddr(lua_State *state)
{
	void *addr;
	auto attr = pthread_attr::to(state, 1);
	int error = pthread_attr_getstackaddr(&attr, &addr);
	if (error) return lux_perror(state, error);
	lua_pushlightuserdata(state, addr);
	return 1;
}
******************************************************************************/

// Set (true) execution stack for new threads
static int setstacksize(lua_State *state)
{
	auto attr = pthread_attr::to(state, 1);
	size_t size = lua_tointeger(state, 2);
	int error = pthread_attr_setstacksize(&attr, size);
	if (error) return lux_perror(state, error);
	return 0;
}

// Get (true) execution stack for new threads
static int getstacksize(lua_State *state)
{
	size_t size;
	auto attr = pthread_attr::to(state, 1);
	int error = pthread_attr_getstacksize(&attr, &size);
	if (error) return lux_perror(state, error);
	lua_pushinteger(state, size);
	return 1;
}

// Class method index for thread attributes
template <> luaL_Reg pthread_attr::index[] =
	{
	{"create", attr_create},
	{"setdetachstate", setdetachstate},
	{"getdetachstate", getdetachstate},
	{"setguardsize", setguardsize},
	{"getguardsize", getguardsize},
	{"setinheritsched", setinheritsched},
	{"getinheritsched", getinheritsched},
	{"setscope", setscope},
	{"getscope", getscope},
	{"setstack", setstack},
	{"getstack", getstack},
//	{"setstackaddr", setstackaddr},
//	{"getstackaddr", getstackaddr},
	{"setstacksize", setstacksize},
	{"getstacksize", getstacksize},
	{nullptr}
	};


/** Mutual Exclusion Attributes **********************************************/


typedef Store<pthread_mutexattr_t> pthread_mutexattr; // Storage class for Lux

// Create mutex attributes object
template <> int pthread_mutexattr::init(lua_State *state, pthread_mutexattr_t &attr)
{
	int error = pthread_mutexattr_init(&attr);
	if (error) return lux_perror(state, error);
	return 1;
}

// Destroy mutex attributes on collection
template <> int pthread_mutexattr::destroy(lua_State *state, pthread_mutexattr_t &attr)
{
	int error = pthread_mutexattr_destroy(&attr);
	if (error) return lux_perror(state, error);
	return 0;
}

// Set the priority attributes for mutexes
static int setprioceiling(lua_State *state)
{
	auto attr = pthread_mutexattr::to(state, 1);
	int prio = lua_tointeger(state, 1);
	int error = pthread_mutexattr_setprioceiling(&attr, prio);
	if (error) return lux_perror(state, error);
	return 0;
}

// Get the priority attribute for mutexes
static int getprioceiling(lua_State *state)
{
	auto attr = pthread_mutexattr::to(state, 1);
	int prio, error = pthread_mutexattr_getprioceiling(&attr, &prio);
	if (error) return lux_perror(state, error);
	lua_pushinteger(state, prio);
	return 1;
}

// Set the protocol attribute for mutexes
static int setprotocol(lua_State *state)
{
	auto attr = pthread_mutexattr::to(state, 1);
	// Single argument must be one of these strings
	const char *opts[] = {"none", "inherit", "protect", nullptr};
	int opt = luaL_checkoption(state, 2, *opts, opts);
	// Convert the option to these POSIX constants
	switch (opt)
	{
	case 0: opt = PTHREAD_PRIO_NONE;
		break;
	case 1: opt = PTHREAD_PRIO_INHERIT;
		break;
	case 2: opt = PTHREAD_PRIO_PROTECT;
		break;
	}
	int error = pthread_mutexattr_setprotocol(&attr, opt);
	if (error) return lux_perror(state, error);
	return 0;
}

// Get the protocol attribute for mutexes
static int getprotocol(lua_State *state)
{
	auto attr = pthread_mutexattr::to(state, 1);
	int opt, error = pthread_mutexattr_getprotocol(&attr, &opt);
	if (error) lux_perror(state, error);
	switch (opt) // Convert to string
	{
	case PTHREAD_PRIO_NONE:
		lua_pushliteral(state, "none");
		break;
	case PTHREAD_PRIO_INHERIT:
		lua_pushliteral(state, "inherit");
		break;
	case PTHREAD_PRIO_PROTECT:
		lua_pushliteral(state, "protect");
		break;
	}
	return 1;
}

// Set the sharing attribute for mutexes
static int setpshared(lua_State *state)
{
	auto attr = pthread_mutexattr::to(state, 1);
	// Single argument must be one of these strings
	const char *opts[] = {"shared", "private", nullptr};
	int opt = luaL_checkoption(state, 2, *opts, opts);
	// Convert the option to one of these POSIX constants
	opt = opt ? PTHREAD_PROCESS_PRIVATE : PTHREAD_PROCESS_SHARED;
	int error = pthread_mutexattr_setpshared(&attr, opt);
	if (error) return lux_perror(state, error);
	lua_pushinteger(state, opt);
	return 1; 
}

// Get the sharing attribute for mutexes
static int getpshared(lua_State *state)
{
	auto attr = pthread_mutexattr::to(state, 1);
	int opt, error = pthread_mutexattr_getpshared(&attr, &opt);
	if (error) return lux_perror(state, error);
	switch (opt)
	{
	case PTHREAD_PROCESS_SHARED:
		lua_pushliteral(state, "shared");
		break;
	case PTHREAD_PROCESS_PRIVATE:
		lua_pushliteral(state, "private");
		break;
	}
	return 1;
}

// Set the type of mutex to create
static int settype(lua_State *state)
{
	auto attr = pthread_mutexattr::to(state, 1);
	// Single argument must be one of these strings
	const char *opts[] = {"default", "recursive", "errorcheck", nullptr};
	int opt = luaL_checkoption(state, 2, *opts, opts);
	// Conver the option to one of these POSIX constants
	switch (opt)
	{
	case 0: opt = PTHREAD_MUTEX_DEFAULT;
		break;
	case 1: opt = PTHREAD_MUTEX_RECURSIVE;
		break;
	case 2: opt = PTHREAD_MUTEX_ERRORCHECK;
		break;
	}
	int error = pthread_mutexattr_settype(&attr, opt);
	if (error) lux_perror(state, error);
	return 0;
}

// Get the type of mutex to create
static int gettype(lua_State *state)
{
	auto attr = pthread_mutexattr::to(state, 1);
	int opt, error = pthread_mutexattr_gettype(&attr, &opt);
	if (error) lux_perror(state, error);
	switch (opt) // Conver to string
	{
	case PTHREAD_MUTEX_DEFAULT:
		lua_pushliteral(state, "default");
		break;
	case PTHREAD_MUTEX_RECURSIVE:
		lua_pushliteral(state, "recursive");
		break;
	case PTHREAD_MUTEX_ERRORCHECK:
		lua_pushliteral(state, "errorcheck");
		break;
	}
	return 1;
}

// Class method index for mutex attributes
template <> luaL_Reg pthread_mutexattr::index[] =
	{
	{"getprioceiling", getprioceiling},
	{"setprioceiling", setprioceiling},
	{"setprotocol", setprotocol},
	{"getprotocol", getprotocol},
	{"getpshared", getpshared},
	{"setpshared", setpshared},
	{"settype", settype},
	{"gettype", gettype},
	{nullptr}
	};


/** Mutual Exclusion *********************************************************/


typedef Store<pthread_mutex_t> pthread_mutex; // Storage class for Lux

// Create a mutual exclusion object
template <> int pthread_mutex::init(lua_State *state, pthread_mutex_t &mutex)
{
	auto user = pthread_mutexattr::test(state);
	auto attr = user ? &user->data : nullptr;
	int error = pthread_mutex_init(&mutex, attr);
	if (error) lux_perror(state, error);
	return 1;
}

// Destroy mutex on garbage collection
template <> int pthread_mutex::destroy(lua_State *state, pthread_mutex_t &mutex)
{
	int error = pthread_mutex_destroy(&mutex);
	if (error) return lux_perror(state, error);
	return 0;
}

// Try locking a mutex, return immediately if cannot
static int mutex_trylock(lua_State *state)
{
	auto mutex = pthread_mutex::to(state, 1);
	int error = pthread_mutex_trylock(&mutex);
	if  (error) // Unsuccessful result
	{
	// Not an error; indicates that a lock is extant
	if (error == EBUSY) lua_pushboolean(state, false);
	// Any other result is an error case
	else return lux_perror(state, error);
	}
	// The mutex has been locked
	else lua_pushboolean(state, true);
	return 1;
}

// Unlock a mutex that is locked
static int mutex_unlock(lua_State *state)
{
	auto mutex = pthread_mutex::to(state, 1);
	int error = pthread_mutex_unlock(&mutex);
	if (error) return lux_perror(state, error);
	return 0;
}

// Wait on mutex until we gain a lock
static int mutex_lock(lua_State *state)
{
	auto mutex = pthread_mutex::to(state, 1);
	int error = pthread_mutex_lock(&mutex);
	if (error) return lux_perror(state, error);
	return 0;
}

// Set the priority attributes for mutexes
static int mutex_setprioceiling(lua_State *state)
{
	auto attr = pthread_mutex::to(state, 1);
	int prio = lua_tointeger(state, 1);
	int error = pthread_mutex_setprioceiling(&attr, prio, &prio);
	if (error) return lux_perror(state, error);
	lua_pushinteger(state, prio);
	return 1;
}

// Get the priority attribute for mutexes
static int mutex_getprioceiling(lua_State *state)
{
	auto attr = pthread_mutex::to(state, 1);
	int prio, error = pthread_mutex_getprioceiling(&attr, &prio);
	if (error) return lux_perror(state, error);
	lua_pushinteger(state, prio);
	return 1;
}

// Class method index for mutual exclusions
template <> luaL_Reg pthread_mutex::index[] =
	{
	{"setprioceiling", mutex_setprioceiling},
	{"getprioceiling", mutex_getprioceiling},
	{"trylock", mutex_trylock},
	{"unlock", mutex_unlock},
	{"lock", mutex_lock},
	{nullptr}
	};


/** Condition Variable Attributes ********************************************/


typedef Store<pthread_condattr_t> pthread_condattr; // Storage class

// Initialize a condition attribute object
template <> int pthread_condattr::init(lua_State *state, pthread_condattr_t &attr)
{
	int error = pthread_condattr_init(&attr);
	if (error) return lux_perror(state, error);
	return 0;
}

// Destroy attributes on garbage collection
template <> int pthread_condattr::destroy(lua_State *state, pthread_condattr_t &attr)
{
	int error = pthread_condattr_destroy(&attr);
	if (error) return lux_perror(state, error);
	return 0;
}

// Set sharing attribute of condition variables
static int condattr_setpshared(lua_State *state)
{
	auto attr = pthread_condattr::to(state, 1);
	// Single argument must be one of these strings
	const char *opts[] = {"private", "shared", nullptr};
	int opt = luaL_checkoption(state, 2, *opts, opts);
	// Conver the option to one of these POSIX constants
	opt = opt ? PTHREAD_PROCESS_SHARED : PTHREAD_PROCESS_PRIVATE;
	int error = pthread_condattr_setpshared(&attr, opt);
	if (error) return lux_perror(state, error);
	return 0;
}

// Get sharing attribute of condition variables
static int condattr_getpshared(lua_State *state)
{
	auto attr = pthread_condattr::to(state, 1);
	int opt, error = pthread_condattr_getpshared(&attr, &opt);
	if (error) return lux_perror(state, error);
	switch (opt) // Convert to string
	{
	case PTHREAD_PROCESS_SHARED:
		lua_pushliteral(state, "shared");
		break;
	case PTHREAD_PROCESS_PRIVATE:
		lua_pushliteral(state, "private");
		break;
	}
	return 1;
}

// Class method index for condition attributes
template <> luaL_Reg pthread_condattr::index[] =
	{
	{"setpshared", condattr_setpshared},
	{"getpshared", condattr_getpshared},
	{nullptr}
	};


/** Condition Variable *******************************************************/


typedef Store<pthread_cond_t> pthread_cond; // Storage class for Lux

// Initialize a condition variable
template <> int pthread_cond::init(lua_State *state, pthread_cond_t &cond)
{
	auto user = pthread_condattr::test(state);
	auto attr = user ? &user->data : nullptr;
	int error = pthread_cond_init(&cond, attr);
	if (error) return lux_perror(state, error);
	return 1;
}

// Destroy condition on collection
template <> int pthread_cond::destroy(lua_State *state, pthread_cond_t &cond)
{
	int error = pthread_cond_destroy(&cond);
	if (error) return lux_perror(state, error);
	return 0;
}

// Wait for condition on locked mutex
static int cond_wait(lua_State *state)
{
	auto cond = pthread_cond::to(state, 1);
	auto mutex = pthread_mutex::to(state, 2);
	int error = pthread_cond_wait(&cond, &mutex);
	if (error) lux_perror(state, error);
	return 0;
}

// Unblock one waiter on this condition
static int cond_signal(lua_State *state)
{
	auto cond = pthread_cond::to(state, 1);
	int error = pthread_cond_signal(&cond);
	if (error) return lux_perror(state, error);
	return 0;
}

// Unblock all waiters on this condition
static int cond_broadcast(lua_State *state)
{
	auto cond = pthread_cond::to(state, 1);
	int error = pthread_cond_broadcast(&cond);
	if (error) return lux_perror(state, error);
	return 0;
}

// Class method index for condition variables
template <> luaL_Reg pthread_cond::index[] =
	{
	{"broadcast", cond_broadcast},
	{"signal", cond_signal},
	{"wait", cond_wait},
	{nullptr}
	};


/** Read/Write Lock Attributes ***********************************************/


typedef Store<pthread_rwlockattr_t> pthread_rwlockattr; // Storage class

// Initialize read/write locker object
template <> int pthread_rwlockattr::init(lua_State *state, pthread_rwlockattr_t &attr)
{
	int error = pthread_rwlockattr_init(&attr);
	if (error) return lux_perror(state, error);
	return 1;
}

// Destroy attributes on garbage collection
template <> int pthread_rwlockattr::destroy(lua_State *state, pthread_rwlockattr_t &attr)
{
	int error = pthread_rwlockattr_init(&attr);
	if (error) lux_perror(state, error);
	return 0;
}

// Set sharing attribute of read/write locker
static int rwlockattr_setpshared(lua_State *state)
{
	auto attr = pthread_rwlockattr::to(state, 1);
	// Single argument must be one of these strings
	const char *opts[] = {"private", "shared", nullptr};
	int opt = luaL_checkoption(state, 2, *opts, opts);
	// Convert the option to one of these POSIX constants
	opt = opt ? PTHREAD_PROCESS_SHARED : PTHREAD_PROCESS_PRIVATE;
	int error = pthread_rwlockattr_setpshared(&attr, opt);
	if (error) return lux_perror(state, error);
	return 0;
}

// Get sharing attribute of read/write locker
static int rwlockattr_getpshared(lua_State *state)
{
	auto attr = pthread_rwlockattr::to(state, 1);
	int opt, error = pthread_rwlockattr_getpshared(&attr, &opt);
	if (error) return lux_perror(state, error);
	switch (opt) // Conver to a string
	{
	case PTHREAD_PROCESS_SHARED:
		lua_pushliteral(state, "shared");
		break;
	case PTHREAD_PROCESS_PRIVATE:
		lua_pushliteral(state, "private");
		break;
	}
	return 1;
}

// Class method index for rwlock attributes
template <> luaL_Reg pthread_rwlockattr::index[] =
	{
	{"setpshared", rwlockattr_setpshared},
	{"getpshared", rwlockattr_getpshared},
	{nullptr}
	};


/** Read/Wright Lock *********************************************************/


typedef Store<pthread_rwlock_t> pthread_rwlock; // Storage class for Lux

// Initialize a read/write locker object
template <> int pthread_rwlock::init(lua_State *state, pthread_rwlock_t &rwlock)
{
	auto user = pthread_rwlockattr::test(state);
	auto attr = user ? &user->data : nullptr;
	int error = pthread_rwlock_init(&rwlock, attr);
	if (error) return lux_perror(state, error);
	return 1;
}

// Destroy object on collection
template <> int pthread_rwlock::destroy(lua_State *state, pthread_rwlock_t &rwlock)
{
	int error = pthread_rwlock_destroy(&rwlock);
	if (error) lux_perror(state, error);
	return 0;
}

// Attempt to aquire a reading lock
static int rwlock_tryrdlock(lua_State *state)
{
	auto rwlock = pthread_rwlock::to(state, 1);
	int error = pthread_rwlock_tryrdlock(&rwlock);
	if (error) return lux_perror(state, error);
	return 0;
}

// Wait to aquire a reading lock
static int rwlock_rdlock(lua_State *state)
{
	auto rwlock = pthread_rwlock::to(state, 1);
	int error = pthread_rwlock_rdlock(&rwlock);
	if (error) return lux_perror(state, error);
	return 0;
}

// Attempt to aquire a writing lock
static int rwlock_trywrlock(lua_State *state)
{
	auto rwlock = pthread_rwlock::to(state, 1);
	int error = pthread_rwlock_trywrlock(&rwlock);
	if (error) return lux_perror(state, error);
	return 0;
}

// Wait to aquire a writing lock
static int rwlock_wrlock(lua_State *state)
{
	auto rwlock = pthread_rwlock::to(state, 1);
	int error = pthread_rwlock_wrlock(&rwlock);
	if (error) return lux_perror(state, error);
	return 0;
}

// Release the lock on this object
static int rwlock_unlock(lua_State *state)
{
	auto rwlock = pthread_rwlock::to(state, 1);
	int error = pthread_rwlock_unlock(&rwlock);
	if (error) return lux_perror(state, error);
	return 0;
}

// Class method index for read/write locker
template <> luaL_Reg pthread_rwlock::index[] =
	{
	{"tryrdlock", rwlock_tryrdlock},
	{"trywrlock", rwlock_trywrlock},
	{"rdlock", rwlock_rdlock},
	{"wrlock", rwlock_wrlock},
	{"unlock", rwlock_unlock},
	{nullptr}
	};


/** Module Loader ************************************************************/


// The Lua C module entry point
extern "C" int luaopen_pthread(lua_State *state)
{
	luaL_Reg regs[] =
	{
	{"create", create},
	{"detach", detach},
	{"equal", equal},
	{"join", join},
	{"self", self},
	{"cancel", cancel},
	{"setcancelstate", setcancelstate},
	{"setcanceltype", setcanceltype},
	{"getconcurrency", getconcurrency},
	{"setconcurrency", setconcurrency},
	{"setschedprio", setschedprio},
	{nullptr}
	};
	luaL_newlib(state, regs);

	luaL_Reg objs[] =
	{
	{"attr", pthread_attr::open},
	{"mutex", pthread_mutex::open},
	{"mutexattr", pthread_mutexattr::open},
	{"cond", pthread_cond::open},
	{"condattr", pthread_condattr::open},
	{"rwlock", pthread_rwlock::open},
	{"rwlockattr", pthread_rwlockattr::open},
	{nullptr}
	};
	for (auto r=objs; r->name; ++r)
	{
		luaL_requiref(state, r->name, r->func, false);
		lua_setfield(state, -2, r->name);
	}
	
	return 1; // pthread table
}

