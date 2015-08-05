#ifndef __lxalloc__
#define __lxalloc__

/**
 * Lua is a garbage collected language so it wants responsibility for memory
 * allocation and freeing. C++ classes are also constructible with the 'new'
 * operator. But C++ also allows you to overload this operator so that there
 * is a way for Lua to 'call' the constructor of a C++ class when it creates
 * memory for an object. To do this, the following overloads will do given a
 * lua_State pointer as an argument.
 *
 *	new (state) ObjectClass(args...);
 *
 * There is no need to capture the return value since this pointer is placed
 * on the stack. The one caveat is that Lua will not call the destructor for
 * your C++ class unless you set it up in the __gc metatable method. If this
 * is not done, any memory that your class allocated will not be freed. Your
 * program will have memory leaks. Using std::shared_ptr will not avoid this
 * problem. You are better off using the higher level constructs in Lux.
 */

#include "lua.hpp"

// Specialisation for memory allocation with class constructor calls

void *operator new (size_t size, lua_State *state);
void *operator new [] (size_t size, lua_State *state);

#endif // file

