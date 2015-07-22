#include "lux.hpp"
#include <cstring>

#define REG(reg) {#reg, lux_cast(reg)},

extern "C" int luaopen_cstring(lua_State *state)
{
	luaL_Reg regs [] =
	{
	REG(memcpy)
//	REG(memchr)
	REG(memcpy)
	REG(memmove)
	REG(memset)
	REG(strcat)
//	REG(strchr)
	REG(strcmp)
	REG(strcoll)
	REG(strcpy)
	REG(strcspn)
	REG(strdup)
	REG(strerror)
	REG(strlen)
	REG(strncat)
	REG(strncmp)
	REG(strncpy)
//	REG(strpbrk)
//	REG(strrchr)
	REG(strspn)
//	REG(strstr)
	REG(strtok)
//	REG(strtok_r)
	REG(strxfrm)
	{nullptr}
	};
	luaL_newlib(state, regs);
	return 1;
}

