#include  "lux.hpp"
#include <csignal>

extern "C" int luaopen_csignal(lua_State *state)
{
	luaL_Reg regs[] =
	{
	{"raise", lux_cast(raise)},
	{"signal", lux_cast(signal)},
	{nullptr}
	};
	luaL_newlib(state, regs);

	lux_Reg<lua_Integer> args[] =
	{
	{"ABRT", SIGABRT},
	{"FPE", SIGFPE},
	{"ILL", SIGILL},
	{"INT", SIGINT},
	{"SEGV", SIGSEGV},
	{"TERM", SIGTERM},
	{nullptr}
	};
	lux_setregs(state, args);

	lux_push(state, SIG_DFL);
	lua_setfield(state, -2, "DFL");
	lux_push(state, SIG_IGN);
	lua_setfield(state, -2, "IGN");
	lux_push(state, SIG_ERR);
	lua_setfield(state, -2, "ERR");

	return 1;
}

