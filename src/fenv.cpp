#include "lux.hpp"
#include <cfenv>

extern "C" int luaopen_fenv(lua_State *state)
{
	// Use the fenv_t struct metatable as the module table
	luaL_requiref(state, "fenv", lux_Class<fenv_t>::open, false);

	// Functions
	luaL_Reg regs[] =
	{
	{"clear", lux_cast(feclearexcept)},
	{"raise", lux_cast(feraiseexcept)},
	{"test", lux_cast(fetestexcept)},
	{"hold", lux_cast(feholdexcept)},
	{"update", lux_cast(feupdateenv)},
	{"getflag", lux_cast(fegetexceptflag)},
	{"setflag", lux_cast(fesetexceptflag)},
	{"getround", lux_cast(fegetround)},
	{"setround", lux_cast(fesetround)},
	{"getenv", lux_cast(fegetenv)},
	{"setenv", lux_cast(fesetenv)},
	{nullptr}
	};
	luaL_setfuncs(state, regs, 0);

	// Parameters
	lux_Reg<lua_Integer> args[] =
	{
	{"DIVBYZERO", FE_DIVBYZERO},
	{"INEXACT", FE_INEXACT},
	{"INVALID", FE_INVALID},
	{"OVERFLOW", FE_OVERFLOW},
	{"UNDERFLOW", FE_UNDERFLOW},
	{"ALL_EXCEPT", FE_ALL_EXCEPT},
	{"DOWNWARD", FE_DOWNWARD},
	{"TONEAREST", FE_TONEAREST},
	{"TOWARDZERO", FE_TOWARDZERO},
	{"UPWARD", FE_UPWARD},
	{nullptr}
	};
	lux_settable(state, args);

	// Default FE environment
	lux_push(state, FE_DFL_ENV);
	lua_setfield(state, -2, "DFL");

	// Done
	return 1;
}

