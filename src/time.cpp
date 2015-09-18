#include "lux.hpp"
#include <ctime>

template <> luaL_Reg lux_Class<tm>::index[] =
{
	{"tm_sec", lux_member(tm, tm_sec)},
	{"tm_min", lux_member(tm, tm_min)},
	{"tm_hour", lux_member(tm, tm_hour)},
	{"tm_mday", lux_member(tm, tm_mday)},
	{"tm_mon", lux_member(tm, tm_mon)},
	{"tm_year", lux_member(tm, tm_year)},
	{"tm_wday", lux_member(tm, tm_wday)},
	{"tm_yday", lux_member(tm, tm_yday)},
	{"tm_isdst", lux_member(tm, tm_isdst)},
	{nullptr}
};

template <> luaL_Reg lux_Class<timespec>::index[] =
{
	{"tv_sec", lux_member(timespec, tv_sec)},
	{"tv_nsec", lux_member(timespec, tv_nsec)},
	{nullptr}
};

extern "C" int luaopen_time(lua_State *state)
{
	luaL_Reg regs[] =
	{
	{"clock", lux_cast(clock)},
	{"difftime", lux_cast(difftime)},
	{"mktime", lux_cast(mktime)},
	{"time", lux_cast(time)},
	{"asctime", lux_cast(asctime)},
	{"ctime", lux_cast(ctime)},
	{"gmtime", lux_cast(gmtime)},
	{"localtime", lux_cast(localtime)},
	{"strftime", lux_cast(strftime)},
	{nullptr}
	};
	luaL_setfuncs(state, regs, 0);
	lua_pushinteger(state, CLOCKS_PER_SEC);
	lua_setfield(state, -2, "CLOCKS_PER_SEC");
	return 1;
}

