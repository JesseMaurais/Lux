#ifndef __lxalloc__
#define __lxalloc__

#include "lua.hpp"

void *operator new (size_t size, lua_State *state);
void *operator new [] (size_t size, lua_State *state);

#endif // file

