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

static int l_fcntl(lua_State *state)
{
	int r = 0;
	int fd = lua_tointeger(state, 1);
	int cmd = lua_tointeger(state, 2);
	switch (cmd)
	{

	}
	lua_pushinteger(state, r);
	return 1;
}

template <> luaL_Reg lux_Class<flock>::index[] =
{
	{"l_type", lux_member(flock, l_type)},
	{"l_whence", lux_member(flock, l_whence)},
	{"l_start", lux_member(flock, l_whence)},
	{"l_len", lux_member(flock, l_len)},
	{"l_pid", lux_member(flock, l_pid)},
	{nullptr}
};

extern "C" int luaopen_fcntl(lua_State *state)
{
	luaL_Reg regs[] =
	{
	{"open", l_open},
	{"creat", l_creat},
	{"fcntl", l_fcntl},
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
	{"F_DUPFD", F_DUPFD},
	{"F_GETFD", F_GETFD},
	{"F_SETFD", F_SETFD},
	{"F_GETLK", F_GETLK},
	{"F_SETLK", F_SETLK},
	{"F_SETLKW", F_SETLKW},
	{"F_GETOWN", F_GETOWN},
	{"F_SETOWN", F_SETOWN},
	{"F_RDLCK", F_RDLCK},
	{"F_UNLCK", F_UNLCK},
	{"F_WRLCK", F_WRLCK},
	{"FD_CLOEXEC", FD_CLOEXEC},
	{nullptr}
	};
	lux_settable(state, args);

	return 1;
}

