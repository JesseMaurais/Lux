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
 *	auto obj = new (state) Type(args...);
 *
 * There is no need to capture the return value since this pointer is placed
 * on the stack. The one caveat is that Lua will not call the destructor for
 * your C++ class unless you set it up in the __gc metatable method. If this
 * is not done, any resources that your class has will not be freed, so that
 * your program might get memory leaks. However, this problem does not occur
 * if your class does not allocate resources (memory) which is the case with
 * ordinary C structs and unions. 
 */

#include "lua.hpp"

// Specialisation for memory allocation with class constructor calls

void *operator new (size_t size, lua_State *state);
void *operator new [] (size_t size, lua_State *state);

#endif // file

