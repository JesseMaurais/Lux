#include "lux.hpp"
#include <cfenv>

extern "C" int luaopen_fenv(lua_State *state)
{
	luaL_requiref(state, "fenv", lux_Class<fenv_t>::open, true);
	luaL_Reg regs[] =
	{
	{"clear", lux_cast(feclearexcept)},
	{"getflag", lux_cast(fegetexceptflag)},
	{"raise", lux_cast(feraiseexcept)},
	{"setflag", lux_cast(fesetexceptflag)},
	{"test", lux_cast(fetestexcept)},
	{"getround", lux_cast(fegetround)},
	{"setround", lux_cast(fesetround)},
	{"getenv", lux_cast(fegetenv)},
	{"hold", lux_cast(feholdexcept)},
	{"setenv", lux_cast(fesetenv)},
	{"update", lux_cast(feupdateenv)},
	{nullptr}
	};
	luaL_setfuncs(state, regs, 0);
	struct {
	 const char *name;
	 lua_Integer value;
	}
	args[] =
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
	for (auto r=args; r->name; ++r)
	{
	 lua_pushinteger(state, r->value);
	 lua_setfield(state, -2, r->name);
	}
	lux_push(state, FE_DFL_ENV);
	lua_setfield(state, -2, "default");
	return 1;
}

