#include "lux.hpp"
#include <fcntl.h>

static int l_open(lua_State *state)
{
	auto path = luaL_checkstring(state, 1);
	int flags = luaL_optinteger(state, 2, O_RDWR|O_CREAT);
	mode_t mode = luaL_optinteger(state, 3, 0777);
	int fd = open(path, flags, mode);
	if (fd < 0) lux_perror(state);
	lux_push(state, fd);
	return 1;		
}

static int l_creat(lua_State *state)
{
	auto path = luaL_checkstring(state, 1);
	mode_t mode = luaL_optinteger(state, 3, 0777);
	int fd = creat(path, mode);
	if (fd < 0) lux_perror(state);
	lux_push(state, fd);
	return 1;
}

extern "C" int luaopen_fcntl(lua_State *state)
{
	luaL_Reg regs[] =
	{
	{"open", l_open},
	{"creat", l_creat},
	{nullptr}
	};
	luaL_newlib(state, regs);

	lux_Reg<lua_Integer> args[] =
	{
	{"O_CREAT", O_CREAT},
	{"O_EXCL", O_EXCL},
	{"O_NOCTTY", O_NOCTTY},
	{"O_TRUNC", O_TRUNC},
	{"O_APPEND", O_APPEND},
	{"O_DSYNC", O_DSYNC},
	{"O_NONBLOCK", O_NONBLOCK},
	{"O_RSYNC", O_RSYNC},
	{"O_SYNC", O_SYNC},
	{"O_ACCMODE", O_ACCMODE},
	{"O_RDONLY", O_RDONLY},
	{"O_RDWR", O_RDWR},
	{"O_WRONLY", O_WRONLY},
	{nullptr}
	};
	lux_settable(state, args);

	return 1;
}

