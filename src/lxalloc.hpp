#ifndef __lxalloc__
#define __lxalloc__

/**
 * Lua is a garbage collected language so it takes responsibility for memory
 * allocation. C++ classes are also constructible with the 'new' operator.
 * But C++ also allows you to overload this operator so that there is a way
 * for Lua to 'call' the constructor of a C++ class when it creates memory for
 * an object. To do this, the following overloads will do given a lua_State
 * pointer as an argument.
 *
 *	auto obj = new (state) Type(args...);
 *
 * There is no need to capture the return value since this pointer is placed
 * on the stack. The one caveat is that Lua will not call the destructor for
 * your C++ class unless you set it up in the __gc metatable method. If this
 * is not done, any resources that your class has allocated will not be freed,
 * so that your program might have memory leaks. This problem does not occur
 * if your class does not allocate resources (like memory) which is the
 * typical case with ordinary C structs and unions. Classes and structs
 * without a default destructor are not necessarily safe, since their member's
 * destructors are called at this same time, but are not when the parent is
 * not. Either your code sets the __gc metamethod to call the destructor or
 * you only allocate POD types with this 'new' overload. Lux storage classes
 * always define the __gc metamethod in this way. 
 */

#include "lua.hpp"
#include <new>

inline void *operator new (std::size_t size, lua_State *state)
{
	return lua_newuserdata(state, size);
}

inline void *operator new [] (std::size_t size, lua_State *state)
{
	return lua_newuserdata(state, size);
}

#endif // file

