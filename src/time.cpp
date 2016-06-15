#include "lux.hpp"
#include <time.h>

// ANSI C

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

static int l_clock(lua_State *state)
{
	lux_push<clock_t>(state, clock());
	return 1;
}

static int l_difftime(lua_State *state)
{
	auto end = lux_to<time_t>(state, 1);
	auto begin = lux_to<time_t>(state, 2);
	lux_push(state, difftime(end, begin));
	return 1;
}

static int l_mktime(lua_State *state)
{
	auto tm = lux_to<struct tm*>(state, 1);
	lux_push<time_t>(state, mktime(tm));
	return 1;
}

static int l_time(lua_State *state)
{
	lux_push<time_t>(state, time(nullptr));
	return 1;
}

static int l_asctime(lua_State *state)
{
	auto tm = lux_to<struct tm*>(state, 1);
	auto str = asctime(tm);
	lua_pushstring(state, str);
	return 1;
}

static int l_ctime(lua_State *state)
{
	auto timer = lux_to<time_t>(state, 1);
	auto str = ctime(&timer);
	lua_pushstring(state, str);
	return 1;
}

static int l_gmtime(lua_State *state)
{
	auto timer = lux_to<time_t>(state, 1);
	lux_push<struct tm*>(state, gmtime(&timer));
	return 1;
}

static int l_localtime(lua_State *state)
{
	auto timer = lux_to<time_t>(state, 1);
	lux_push(state, localtime(&timer));
	return 1;
}

static int l_strftime(lua_State *state)
{
	char buf[BUFSIZ];
	auto fmt = lua_tostring(state, 1);
	auto tm = lux_to<struct tm*>(state, 2);
	size_t sz = strftime(buf, sizeof(buf), fmt, tm);
	if (sz > 0)
	{
	 lua_pushlstring(state, buf, sz);
	 return 1;
	}
	return 0;
}

// POSIX clocks

template <> luaL_Reg lux_Class<timespec>::index[] =
{
	{"tv_sec", lux_member(timespec, tv_sec)},
	{"tv_nsec", lux_member(timespec, tv_nsec)},
	{nullptr}
};

static int l_clock_getcpuclockid(lua_State *state)
{
	clockid_t clockid;
	auto pid = lux_to<pid_t>(state, 1);
	int error = clock_getcpuclockid(pid, &clockid);
	if (error) return lux_perror(state, error);
	lux_push<clockid_t>(state, clockid);
	return 1;	
}

static int l_clock_getres(lua_State *state)
{
	// Storage class for the timespec
	using Type = lux_Class<timespec>::Type;
	// Clock from which to fill the timespec
	auto clockid = lux_to<clockid_t>(state, 1);
	// Create an object with storage
	auto data = new (state) timespec;
	luaL_setmetatable(state, Type::name);
	// Call the POSIX function, error check
	int error = clock_getres(clockid, data);
	if (error) lux_perror(state, error);
	// Result is timespec
	return 1;
}

static int l_clock_gettime(lua_State *state)
{
	// Storage class for the timespec
	using Type = lux_Class<timespec>::Type;
	// Clock from which to fill the timespec
	auto clockid = lux_to<clockid_t>(state, 1);
	// Create an object with storage
	auto data = new (state) timespec;
	luaL_setmetatable(state, Type::name);
	// Call the POSIX function, error check
	int error = clock_gettime(clockid, data);
	if (error) lux_perror(state, error);
	// Result is timespec
	return 1;
}

static int l_clock_settime(lua_State *state)
{
	auto clockid = lux_to<clockid_t>(state, 1);
	auto spec = lux_to<timespec>(state, 2);
	int error = clock_settime(clockid, &spec);
	if (error) lux_perror(state, error);
	return 1;
}

// POSIX timers

template <> luaL_Reg lux_Class<itimerspec>::index[] =
{
	{"it_interval", lux_member(itimerspec, it_interval)},
	{"it_value", lux_member(itimerspec, it_value)},
	{nullptr}
};

static int l_timer_create(lua_State *state)
{
	timer_t timerid;
	auto clockid = lux_to<clockid_t>(state, 1);
	int error = timer_create(clockid, nullptr, &timerid);
	if (error) lux_perror(state);
	lux_push(state, timerid);
	return 1;
}

static int l_timer_delete(lua_State *state)
{
	auto timerid = lux_to<timer_t>(state, 1);
	int error = timer_delete(timerid);
	if (error) lux_perror(state);
	return 0;
}

static int l_timer_getoverrun(lua_State *state)
{
	auto timerid = lux_to<timer_t>(state, 1);
	int overrun = timer_getoverrun(timerid);
	if (overrun < 0) lux_perror(state);
	lua_pushinteger(state, overrun);
	return 1;
}

static int l_timer_gettime(lua_State *state)
{
	// Storage class for itimerspec
	using Type = lux_Class<itimerspec>::Type;
	// Time from which to fill the itimerspec
	auto timerid = lux_to<timer_t>(state, 1);
	// Create an object with storage
	auto data = new (state) itimerspec;
	luaL_setmetatable(state, Type::name);
	// Call the POSIX function, check error
	int error = timer_gettime(timerid, data);
	if (error) lux_perror(state);
	// Result is itimerspec
	return 1;
}

static int l_timer_settime(lua_State *state)
{
	auto timerid = lux_to<timer_t>(state, 1);
	auto old = lux_to<itimerspec*>(state, 2);
	auto spec = *old;
	int error = timer_settime(timerid, TIMER_ABSTIME, &spec, old);
	if (error) lux_perror(state);
	return 0;
}

// Lua module entry point

extern "C" int luaopen_time(lua_State *state)
{
	luaL_requiref(state, "tm", lux_Class<tm>::open, false);
	luaL_requiref(state, "timespec", lux_Class<timespec>::open, false);
	luaL_requiref(state, "itimerspec", lux_Class<itimerspec>::open, false);

	luaL_Reg regs[] =
	{
	{"clock", l_clock},
	{"difftime", l_difftime},
	{"mktime", l_mktime},
	{"time", l_time},
	{"asctime", l_asctime},
	{"ctime", l_ctime},
	{"gmtime", l_gmtime},
	{"localtime", l_localtime},
	{"strftime", l_strftime},
	{"clock_getcpuclockid", l_clock_getcpuclockid},
	{"clock_getres", l_clock_getres},
	{"clock_gettime", l_clock_gettime},
	{"clock_settime", l_clock_settime},
	{"timer_create", l_timer_create},
	{"timer_delete", l_timer_delete},
	{"timer_getoverrun", l_timer_getoverrun},
	{"timer_gettime", l_timer_gettime},
	{"timer_settime", l_timer_settime},
	{nullptr}
	};
	luaL_newlib(state, regs);

	lux_Reg<lua_Integer> args[] =
	{
	{"CLOCKS_PER_SEC", CLOCKS_PER_SEC},
	{"CLOCK_REALTIME", CLOCK_REALTIME},
	{"CLOCK_MONOTONIC", CLOCK_MONOTONIC},
	{nullptr}
	};
	lux_settable(state, args);

	return 1;
}

