/**
 * Support for C signal handling. This is a rather limited implementation. It does not
 * allow Lua functions to be set as the signal handlers. What it does allow is for the
 * script to set the behaviour of signals by switching between DFL, IGN, and ERR.
 */

#include  "lux.hpp"
#include <csignal>

template <> inline
void lux_push<sighandler_t>(lua_State *state, sighandler_t act)
{
	lua_pushlightuserdata(state, (void*) act);
}

template <> inline
sighandler_t lux_to<sighandler_t>(lua_State *state, int stack)
{
	return (sighandler_t) lua_touserdata(state, stack);
}

static int setup(lua_State *state)
{
	int sig = lua_tointeger(state, 1);
	auto act = lux_to<sighandler_t>(state, 2);
	signal(sig, act);
	return 0;
}

extern "C" int luaopen_csignal(lua_State *state)
{
	luaL_Reg regs[] =
	{
	{"raise", lux_cast(raise)},
	{"signal", setup},
	{nullptr}
	};
	luaL_newlib(state, regs);

	lux_Reg<lua_Integer> sigs[] =
	{
	{"HUP",     1},	// POSIX
	{"INT",     2},	// ANSI
	{"QUIT",    3},	// POSIX
	{"ILL",     4},	// ANSI
	{"TRAP",    5},	// POSIX
	{"ABRT",    6},	// ANSI
	{"IOT",     6},	// 4.2BSD
	{"BUS",     7},	// 4.2BSD
	{"FPE",     8},	// ANSI
	{"KILL",    9},	// POSIX
	{"USR1",   10},
	{"SEGV",   11},	// ANSI
	{"USR2",   12},
	{"PIPE",   13},	// POSIX
	{"ALRM",   14},	// ANSI
	{"TERM",   15},	// TERM
	{"STKFLT", 16},	// ANSI
	{"CHLD",   17},	// POSIX
	{"CONT",   18},	// POSIX
	{"STOP",   19},	// POSIX
	{"TSTP",   20},	// POSIX
	{"TTIN",   21},	// POSIX
	{"TTOU",   22},	// POSIX
	{"URG",    23},	// 4.2BSD
	{"XCPU",   24},	// 4.2BSD
	{"XFSZ",   25},	// 4.2BSD
	{"VTALRM", 26},	// 4.2BSD
	{"PROF",   27},	// 4.2BSD, SUN
	{"WINCH",  28},	// 4.3BSD, SUN
	{"IO",     29},	// 4.2BSD
	{"POLL",   29},	// SysV
	{"PWR",    30},	// SysV
	{"SYS",    31},
	{nullptr}
	};
	lux_settable(state, sigs);

	lux_Reg<sighandler_t> acts[] =
	{
	{"DFL", SIG_DFL},
	{"IGN", SIG_IGN},
	{"ERR", SIG_ERR},
	{nullptr}
	};
	lux_settable(state, acts);

	return 1;
}

