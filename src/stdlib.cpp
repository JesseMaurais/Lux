#include "lux.hpp"
#include <cstdlib>
#include <tuple>

static int _div(lua_State *state)
{
	auto numer = lux_to<int>(state, 1);
	auto denom = lux_to<int>(state, 2);
	div_t res = div(numer, denom);
	return lux_push(state, res.quot, res.rem);
}

static int _ldiv(lua_State *state)
{
	auto numer = lux_to<long>(state, 1);
	auto denom = lux_to<long>(state, 2);
	ldiv_t res = ldiv(numer, denom);
	return lux_push(state, res.quot, res.rem);
}

static int _lldiv(lua_State *state)
{
	auto numer = lux_to<long long>(state, 1);
	auto denom = lux_to<long long>(state, 2);
	lldiv_t res = lldiv(numer, denom);
	return lux_push(state, res.quot, res.rem);
}

#define REG(reg) {#reg, lux_cast(reg)},
#define ARG(arg) {#arg, arg},

extern "C" int luaopen_stdlib(lua_State *state)
{
	luaL_Reg regs[] =
	{
	REG(_Exit)
	REG(a64l)
	REG(abort)
	REG(abs)
//	REG(atexit)
	REG(atof)
	REG(atoi)
	REG(atol)
	REG(atoll)
//	REG(bsearch)
	REG(calloc)
	{"div", _div},
	REG(drand48)
//	REG(ecvt)
	REG(erand48)
	REG(exit)
//	REG(fcvt)
	REG(free)
//	REG(gcvt)
	REG(getenv)
	REG(getsubopt)
	REG(grantpt)
	REG(initstate)
	REG(jrand48)
	REG(l64a)
	REG(labs)
	REG(lcong48)
	{"ldiv", _ldiv},
	REG(llabs)
	{"lldiv", _lldiv},
	REG(lrand48)
	REG(malloc)
	REG(mblen)
	REG(mbstowcs)
	REG(mbtowc)
//	REG(mktemp)
	REG(mkstemp)
	REG(mrand48)
	REG(nrand48)
	REG(posix_memalign)
	REG(posix_openpt)
	REG(ptsname)
	REG(putenv)
//	REG(qsort)
	REG(rand)
	REG(rand_r)
	REG(random)
	REG(realloc)
	REG(realpath)
	REG(seed48)
	REG(setenv)
//	REG(setkey)
	REG(setstate)
	REG(srand)
	REG(srand48)
	REG(srandom)
//	REG(strtod)
//	REG(strtof)
//	REG(strtol)
//	REG(strtold)
//	REG(strtoll)
//	REG(strtoul)
//	REG(strtoull)
	REG(system)
	REG(unlockpt)
	REG(unsetenv)
	REG(wcstombs)
	REG(wctomb)
	{nullptr}
	};
	luaL_newlib(state, regs);

	lux_Reg<lua_Integer> args[] =
	{
	ARG(EXIT_FAILURE)
	ARG(EXIT_SUCCESS)
	ARG(RAND_MAX)
	ARG(MB_CUR_MAX)
	{nullptr}
	};
	lux_settable(state, args);

	return 1;
}

