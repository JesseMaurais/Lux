#include "lux.hpp"
#include <regex.h>

typedef lux_Store<regex_t> Type;

static int option(lua_State *state, int arg)
{
	const char *opts[] =
	{
	"none",
	"extended",
	"icase",
	"nosub",
	"newline",
	"no begin",
	"no end",
	nullptr
	};
	switch (luaL_checkoption(state, arg, *opts, opts))
	{
	case 0: return 0;
	case 1: return REG_EXTENDED;
	case 2: return REG_ICASE;
	case 3: return REG_NOSUB;
	case 4: return REG_NEWLINE;
	case 5: return REG_NOTBOL;
	case 6: return REG_NOTEOL;
	};
}

static int __new(lua_State *state)
{
	regex_t regex;
	auto pattern = luaL_checkstring(state, 1);
	int cflags = 0, top = lua_gettop(state);
	for (int arg = 2; arg < top; ++arg)
	{
		cflags |= option(state, arg);
	}
	int error = regcomp(&regex, pattern, cflags);
	if (error < 0)
	{
		char string[100];
		regerror(error, &regex, string, sizeof(string));
		return luaL_error(state, "%s", string);
	}
	Type::push(state, regex);
	return 1;
}

static int __gc(lua_State *state)
{
	auto &regex = Type::to(state);
	regfree(&regex);
	return 0;
}

static int __len(lua_State *state)
{
	auto &regex = Type::to(state);
	lua_pushinteger(state, regex.re_nsub);
	return 1;
}

static int __call(lua_State *state)
{
	auto &regex = Type::to(state, 1);
	auto string = luaL_checkstring(state, 2);
	int eflags = 0, top = lua_gettop(state);
	for (int arg = 3; arg < top; ++arg)
	{
		eflags |= option(state, arg);
	}
	const size_t size = 100;
	regmatch_t matches[size];
	int error = regexec(&regex, string, size, matches, eflags);
	if (!error)
	{
		lua_newtable(state);
		for (int item = 0; item < size; ++item)
		{
			auto &match = matches[item];
			if (match.rm_so < 0) break;
			lua_pushinteger(state, match.rm_eo-1);
			lua_seti(state, -2, match.rm_so+1);
		}
		return 1;
	}
	else
	if (error == REG_NOMATCH)
	{
		lua_pushnil(state);
		return 1;
	}
	else
	{
		char string[100];
		regerror(error, &regex, string, sizeof(string));
		return luaL_error(state, "%s", string);
	}
}

extern "C" int luaopen_regex(lua_State *state)
{
	Type::name = lua_tostring(state, 1);
	luaL_newmetatable(state, Type::name);

	lua_pushliteral(state, "new");
	lua_pushcfunction(state, __new);
	lua_settable(state, -3);

	lua_pushliteral(state, "__gc");
	lua_pushcfunction(state, __gc);
	lua_settable(state, -3);

	lua_pushliteral(state, "__len");
	lua_pushcfunction(state, __len);
	lua_settable(state, -3);

	lua_pushliteral(state, "__call");
	lua_pushcfunction(state, __call);
	lua_settable(state, -3);
	
	return 1;
}

