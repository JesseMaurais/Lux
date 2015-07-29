#include "lux.hpp"
#include <cfloat>
#include <cfenv>

extern "C" int luaopen_cfenv(lua_State *state)
{
	luaL_requiref(state, "fenv", lux_Class<fenv_t>::open, false);

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
	{"RADIX", FLT_RADIX},
	{"ROUNDS", FLT_ROUNDS},
	{"EVAL_METHOD", FLT_EVAL_METHOD},
	{"DECIMAL_DIG", DECIMAL_DIG},
	{"FLT_MANT_DIG", FLT_MANT_DIG},
	{"DBL_MANT_DIG", FLT_MANT_DIG},
	{"LDBL_MANT_DIG", LDBL_MANT_DIG},
	{"FLT_DIG", FLT_DIG},
	{"DBL_DIG", DBL_DIG},
	{"LDBL_DIG", LDBL_DIG},
	{"FLT_MIN_EXP", FLT_MIN_EXP},
	{"DBL_MIN_EXP", DBL_MIN_EXP},
	{"LDBL_MIN_EXP", LDBL_MIN_EXP},
	{"FLT_MIN_1O_EXP", FLT_MIN_10_EXP},
	{"DBL_MIN_10_EXP", DBL_MIN_10_EXP},
	{"LDBL_MIN_10_EXP", LDBL_MIN_10_EXP},
	{"FLT_MAX_EXP", FLT_MAX_EXP},
	{"DBL_MAX_EXP", DBL_MAX_EXP},
	{"LDBL_MAX_EXP", LDBL_MAX_EXP},
	{"FLT_MAX_10_EXP", FLT_MAX_10_EXP},
	{"DBL_MAX_10_EXP", DBL_MAX_10_EXP},
	{"LDBL_MAX_10_EXP", LDBL_MAX_10_EXP},
	{nullptr}
	};
	lux_settable(state, args);

	lux_Reg<lua_Number> vars[] =
	{
	{"FLT_EPSILON", FLT_EPSILON},
	{"DBL_EPSILON", DBL_EPSILON},
	{"LDBL_EPSILON", LDBL_EPSILON},
	{nullptr}
	};
	lux_settable(state, vars);

	lux_push(state, FE_DFL_ENV);
	lua_setfield(state, -2, "DFL");

	return 1;
}

