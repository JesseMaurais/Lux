#include "lux.hpp"
#include <clocale>

template <> luaL_Reg lux_Class<lconv>::index[] =
{
	{"decimal_point", lux_member(lconv, decimal_point)},
	{"thousands_sep", lux_member(lconv, thousands_sep)},
	{"grouping", lux_member(lconv, grouping)},
	{"int_curr_symbol", lux_member(lconv, int_curr_symbol)},
	{"currency_symbol", lux_member(lconv, currency_symbol)},
	{"mon_decimal_point", lux_member(lconv, mon_decimal_point)},
	{"mon_thousands_sep", lux_member(lconv, mon_thousands_sep)},
	{"mon_grouping", lux_member(lconv, mon_grouping)},
	{"positive_sign", lux_member(lconv, positive_sign)},
	{"negative_sign", lux_member(lconv, negative_sign)},
	{"frac_digits", lux_member(lconv, frac_digits)},
	{"p_cs_precedes", lux_member(lconv, p_cs_precedes)},
	{"n_cs_precedes", lux_member(lconv, n_cs_precedes)},
	{"p_sep_by_space", lux_member(lconv, p_sep_by_space)},
	{"n_sep_by_space", lux_member(lconv, n_sep_by_space)},
	{"p_sign_posn", lux_member(lconv, p_sign_posn)},
	{"n_sign_posn", lux_member(lconv, n_sign_posn)},
	{"int_frac_digits", lux_member(lconv, int_frac_digits)},
	{"int_p_cs_precedes", lux_member(lconv, int_p_cs_precedes)},
	{"int_n_cs_precedes", lux_member(lconv, int_n_cs_precedes)},
	{"int_p_sep_by_space", lux_member(lconv, int_p_sep_by_space)},
	{"int_n_sep_by_space", lux_member(lconv, int_n_sep_by_space)},
	{"int_p_sign_posn", lux_member(lconv, int_p_sign_posn)},
	{"int_n_sign_posn", lux_member(lconv, int_n_sign_posn)},
	{nullptr}
};

extern "C" int luaopen_clocale(lua_State *state)
{
	luaL_requiref(state, "lconv", lux_Class<lconv>::open, false);
	luaL_Reg regs[] =
	{
	{"localeconv", lux_cast(localeconv)},
	{"setlocale", lux_cast(setlocale)},
	{nullptr}
	};
	luaL_setfuncs(state, regs, 0);
	lux_Reg<lua_Integer> args[] =
	{
	{"ALL", LC_ALL},
	{"COLLATE", LC_COLLATE},
	{"CTYPE", LC_CTYPE},
	{"MONETARY", LC_MONETARY},
	{"NUMERIC", LC_NUMERIC},
	{"TIME", LC_TIME},
	{nullptr}
	};
	lux_settable(state, args);
	return 1;
}

