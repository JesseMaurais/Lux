#include  "lux.hpp"
#include <csignal>

template <> void lux_push<sighandler_t>(lua_State *state, sighandler_t sig)
{
	lua_pushlightuserdata(state, (void*) sig);
}

template <> sighandler_t lux_to<sighandler_t>(lua_State *state, int stack)
{
	return (sighandler_t) lua_touserdata(state, stack);
}

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
	lux_settable(state, args);

	lux_push(state, SIG_DFL);
	lua_setfield(state, -2, "DFL");
	lux_push(state, SIG_IGN);
	lua_setfield(state, -2, "IGN");
	lux_push(state, SIG_ERR);
	lua_setfield(state, -2, "ERR");

	return 1;
}

