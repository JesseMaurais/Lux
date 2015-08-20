#include "lux.hpp"
#include <pthread.h>
#include <cstdlib>

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
	pthread_t thread;
	// Create an independent execution stack
	lua_State *stack = lua_newthread(state);
	lua_pop(state, 1);
	// Copy the stack contents over to thread
	lua_xmove(state, stack, lua_gettop(state));
	// Launch thread but long jump if an error occurs
	int error = pthread_create(&thread, nullptr, start, stack);
	if (error) return lux_perror(state, error);
	// Return the thread id
	lux_push(state, thread);
	return 1;
}

// Detach calling thread from main
static int detach(lua_State *state)
{
	auto thread = lux_to<pthread_t>(state, 1);
	int error = pthread_detach(thread);
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
	auto thread = lux_to<pthread_t>(state, 1);
	// Join, getting the execution stack it returns
	int error = pthread_join(thread, (void**) &stack);
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
	lux_push(state, pthread_self());
	return 1;
}

// Cancels execution of a thread
static int cancel(lua_State *state)
{
	auto thread = lux_to<pthread_t>(state, 1);
	int error = pthread_cancel(thread);
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
	lua_pushinteger(state, pthread_getconcurrency());
	return 1;
}

// Attempt to set the scheduling priority
static int setschedprio(lua_State *state)
{
	auto thread = lux_to<pthread_t>(state, 1);
	int prio = lua_tointeger(state, 2);
	int error = pthread_setschedprio(thread, prio);
	if (error) return lux_perror(state, error);
	return 0;
}

/*
// Cleanup routine for a thread
static void routine(void *stack)
{
	lua_State *state = (lua_State*) stack;
	// Assume there is a function and arguments on it
	lua_call(state, lua_gettop(state) - 1, LUA_MULTRET);
}

// Register a cleanup function for a thread
static int cleanup_push(lua_State *state)
{
	// Create an independent execution stack
	lua_State *stack = lua_newthread(state);
	lua_pop(state, 1);
	// Move stack contents over to new thread
	lua_xmove(state, stack, lua_gettop(state));
	// Set this function to be called later
	pthread_cleanup_push(routine, stack);
	return 0;
}

// Remove last cleanup with optional call
static int cleanup_pop(lua_State *state)
{
	bool execute = lua_toboolean(state, 1);
	pthread_cleanup_pop(execute);
	return 0;
}
*/


typedef lux_Store<pthread_attr_t> pthread_attr; // Storage class for Lux


// Initialize a thread attributes type
static int attr_init(lua_State *state)
{
	auto attr = new (state) pthread_attr;
	luaL_setmetatable(state, pthread_attr::name);
	int error = pthread_attr_init(&attr->data);
	if (error) return lux_perror(state, error);
	return 1;
}

// Destroy attributes on collection
static int attr_gc(lua_State *state)
{
	auto attr = pthread_attr::to(state, 1);
	int error = pthread_attr_destroy(&attr);
	if (error) return lux_perror(state, error);
	return 0;
}

// Transform pthread_attr type to string
static int attr_tostring(lua_State *state)
{
	auto attr = pthread_attr::to(state, 1);
	lua_pushfstring(state, "%s: %p", pthread_attr::name, &attr);
	return 1;
}

// Create a thread with attributes
static int attr_create(lua_State *state)
{
	pthread_t thread;
	auto attr = pthread_attr::to(state, 1);
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
	auto attr = pthread_attr::to(state, 1);
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


typedef lux_Store<pthread_mutexattr_t> pthread_mutexattr; // Storage class


// Create mutex attributes object
static int mutexattr_init(lua_State *state)
{
	auto attr = new (state) pthread_mutexattr;
	luaL_setmetatable(state, pthread_mutexattr::name);
	int error = pthread_mutexattr_init(&attr->data);
	if (error) return lux_perror(state, error);
	return 1;
}

// Destroy mutexattr on collection
static int mutexattr_gc(lua_State *state)
{
	auto attr = pthread_mutexattr::to(state, 1);
	int error = pthread_mutexattr_destroy(&attr);
	if (error) return lux_perror(state, error);
	return 0;
}

// Convert to string for printing
static int mutexattr_tostring(lua_State *state)
{
	auto attr = pthread_mutexattr::to(state, 1);
	lua_pushfstring(state, "%s: %p", pthread_mutexattr::name, &attr);
	return 1;
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


typedef lux_Store<pthread_mutex_t> pthread_mutex; // Storage class for Lux


// Create a mutual exclusion object
static int mutex_init(lua_State *state)
{
	auto mutex = new (state) pthread_mutex;
	luaL_setmetatable(state, pthread_mutex::name);
	auto mutexattr = pthread_mutexattr::test(state, 1);
	auto attr = mutexattr ? &mutexattr->data : nullptr;
	int error = pthread_mutex_init(&mutex->data, attr);
	if (error) lux_perror(state, error);
	return 1;
}

// Convert to string for printing
static int mutex_tostring(lua_State *state)
{
	auto mutex = pthread_mutex::to(state, 1);
	lua_pushfstring(state, "%s: %p", pthread_mutex::name, &mutex);
	return 1;
}

// Destroy mutex on garbage collection
static int mutex_gc(lua_State *state)
{
	auto mutex = pthread_mutex::to(state, 1);
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


typedef lux_Store<pthread_condattr_t> pthread_condattr; // Storage class


// Initialize a condition attribute object
static int condattr_init(lua_State *state)
{
	auto attr = new (state) pthread_condattr;
	luaL_setmetatable(state, pthread_condattr::name);
	int error = pthread_condattr_init(&attr->data);
	if (error) return lux_perror(state, error);
	return 0;
}

// Destroy attributes on garbage collection
static int condattr_gc(lua_State *state)
{
	auto attr = pthread_condattr::to(state, 1);
	int error = pthread_condattr_destroy(&attr);
	if (error) return lux_perror(state, error);
	return 0;
}

// Conver to string for printing
static int condattr_tostring(lua_State *state)
{
	auto attr = pthread_condattr::to(state, 1);
	lua_pushfstring(state, "%s: %p", pthread_condattr::name, &attr);
	return 1;
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


typedef lux_Store<pthread_cond_t> pthread_cond; // Storage class for Lux


// Initialize a condition variable
static int cond_init(lua_State *state)
{
	auto cond = new (state) pthread_cond;
	luaL_setmetatable(state, pthread_cond::name);
	auto condattr = pthread_condattr::test(state, 1);
	auto attr = condattr ? &condattr->data : nullptr;
	int error = pthread_cond_init(&cond->data, attr);
	if (error) return lux_perror(state, error);
	return 1;
}

// Destroy condition on collection
static int cond_gc(lua_State *state)
{
	auto cond = pthread_cond::to(state, 1);
	int error = pthread_cond_destroy(&cond);
	if (error) return lux_perror(state, error);
	return 0;
}

// Convert to string for printing
static int cond_tostring(lua_State *state)
{
	auto cond = pthread_cond::to(state, 1);
	lua_pushfstring(state, "%s: %p", pthread_cond::name, &cond);
	return 1;
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


typedef lux_Store<pthread_rwlockattr_t> pthread_rwlockattr; // Storage class


// Initialize read/write locker object
static int rwlockattr_init(lua_State *state)
{
	auto attr = new (state) pthread_rwlockattr;
	luaL_setmetatable(state, pthread_rwlockattr::name);
	int error = pthread_rwlockattr_init(&attr->data);
	if (error) return lux_perror(state, error);
	return 1;
}

// Destroy attributes on garbage collection
static int rwlockattr_gc(lua_State *state)
{
	auto attr = pthread_rwlockattr::to(state, 1);
	int error = pthread_rwlockattr_init(&attr);
	if (error) lux_perror(state, error);
	return 0;
}

// Convert to string for printing
static int rwlockattr_tostring(lua_State *state)
{
	auto attr = pthread_rwlockattr::to(state, 1);
	lua_pushfstring(state, "%s: %p", pthread_rwlockattr::name, &attr);
	return 1;
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


typedef lux_Store<pthread_rwlock_t> pthread_rwlock; // Storage class for Lux


// Initialize a read/write locker object
static int rwlock_init(lua_State *state)
{
	auto rwlock = new (state) pthread_rwlock;
	luaL_setmetatable(state, pthread_rwlock::name);
	auto rwlockattr = pthread_rwlockattr::test(state, 1);
	auto attr = rwlockattr ? &rwlockattr->data : nullptr;
	int error = pthread_rwlock_init(&rwlock->data, attr);
	if (error) return lux_perror(state, error);
	return 1;
}

// Destroy object on collection
static int rwlock_gc(lua_State *state)
{
	auto rwlock = pthread_rwlock::to(state, 1);
	int error = pthread_rwlock_destroy(&rwlock);
	if (error) lux_perror(state, error);
	return 0;
}

// Convert to string for printing
static int rwlock_tostring(lua_State *state)
{
	auto rwlock = pthread_rwlock::to(state, 1);
	lua_pushfstring(state, "%s: %p", pthread_rwlock::name, &rwlock);
	return 1;
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


// Main waits on threads
static void onexit(void)
{
	int value = EXIT_SUCCESS;
	pthread_exit(&value);
}

// The Lua C module entry point
extern "C" int luaopen_pthread(lua_State *state)
{
	// Ensure that we can wait on child threads before program terminates
	if (atexit(onexit) < 0) return luaL_error(state, "Cannot make exit");

	// pthread

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
//	{"cleanup_push", cleanup_push},
//	{"cleanup_pop", cleanup_pop},
	{nullptr}
	};
	luaL_newlib(state, regs);

	// pthread_attr

	luaL_newmetatable(state, pthread_attr::name);
	luaL_Reg attr[] =
	{
	{"init", attr_init},
	{"__tostring", attr_tostring},
	{"__gc", attr_gc},
	{nullptr}
	};
	luaL_setfuncs(state, attr, 0);
	luaL_Reg attr_index[] =
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
	luaL_newlib(state, attr_index);
	lua_setfield(state, -2, "__index");
	lua_setfield(state, -2, "attr");

	// pthread_mutex

	luaL_newmetatable(state, pthread_mutex::name);
	luaL_Reg mutex_regs[] =
	{
	{"init", mutex_init},
	{"__tostring", mutex_tostring},
	{"__gc", mutex_gc},
	{nullptr}
	};
	luaL_setfuncs(state, mutex_regs, 0);
	luaL_Reg mutex_index[] =
	{
	{"setprioceiling", mutex_setprioceiling},
	{"getprioceiling", mutex_getprioceiling},
	{"trylock", mutex_trylock},
	{"unlock", mutex_unlock},
	{"lock", mutex_lock},
	{nullptr}
	};
	luaL_newlib(state, mutex_index);
	lua_setfield(state, -2, "__index");
	lua_setfield(state, -2, "mutex");

	// pthread_mutexattr

	luaL_newmetatable(state, pthread_mutexattr::name);
	luaL_Reg mutexattr_regs[] =
	{
	{"init", mutexattr_init},
	{"__tostring", mutexattr_tostring},
	{"__gc", mutexattr_gc},
	{nullptr}
	};
	luaL_setfuncs(state, mutexattr_regs, 0);
	luaL_Reg mutexattr_index[] =
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
	luaL_newlib(state, mutexattr_index);
	lua_setfield(state, -2, "__index");
	lua_setfield(state, -2, "mutexattr");

	// pthread_cond

	luaL_newmetatable(state, pthread_cond::name);
	luaL_Reg cond_regs[] =
	{
	{"init", cond_init},
	{"__tostring", cond_tostring},
	{"__gc", cond_gc},
	{nullptr}
	};
	luaL_setfuncs(state, cond_regs, 0);
	luaL_Reg cond_index[] =
	{
	{"broadcast", cond_broadcast},
	{"signal", cond_signal},
	{"wait", cond_wait},
	{nullptr}
	};
	luaL_newlib(state, cond_index);
	lua_setfield(state, -2, "__index");
	lua_setfield(state, -2, "cond");

	// pthread_condattr

	luaL_newmetatable(state, pthread_condattr::name);
	luaL_Reg condattr_regs[] =
	{
	{"init", condattr_init},
	{"__tostring", condattr_tostring},
	{"__gc", condattr_gc},
	{nullptr}
	};
	luaL_setfuncs(state, condattr_regs, 0);
	luaL_Reg condattr_index[] =
	{
	{"setpshared", condattr_setpshared},
	{"getpshared", condattr_getpshared},
	{nullptr}
	};
	luaL_newlib(state, condattr_index);
	lua_setfield(state, -2, "__index");
	lua_setfield(state, -2, "condattr");

	// pthread_rwlock

	luaL_newmetatable(state, pthread_rwlock::name);
	luaL_Reg rwlock_regs[] =
	{
	{"init", rwlock_init},
	{"__tostring", rwlock_tostring},
	{"__gc", rwlock_gc},
	{nullptr}
	};
	luaL_setfuncs(state, rwlock_regs, 0);
	luaL_Reg rwlock_index[] =
	{
	{"tryrdlock", rwlock_tryrdlock},
	{"trywrlock", rwlock_trywrlock},
	{"rdlock", rwlock_rdlock},
	{"wrlock", rwlock_wrlock},
	{"unlock", rwlock_unlock},
	{nullptr}
	};
	luaL_newlib(state, rwlock_index);
	lua_setfield(state, -2, "__index");
	lua_setfield(state, -2, "rwlock");

	// pthread_wrlockattr

	luaL_newmetatable(state, pthread_rwlockattr::name);
	luaL_Reg rwlockattr_regs[] =
	{
	{"init", rwlockattr_init},
	{"__tostring", rwlockattr_tostring},
	{"__gc", rwlockattr_gc},
	{nullptr}
	};
	luaL_setfuncs(state, rwlockattr_regs, 0);
	luaL_Reg rwlockattr_index[] =
	{
	{"setpshared", rwlockattr_setpshared},
	{"getpshared", rwlockattr_getpshared},
	{nullptr}
	};
	luaL_newlib(state, rwlockattr_index);
	lua_setfield(state, -2, "__index");
	lua_setfield(state, -2, "rwlockattr");

	
	return 1; // pthread table
}

