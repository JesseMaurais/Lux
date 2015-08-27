#include "lux.hpp"
#include <regex>

using namespace std::regex_constants;

static syntax_option_type optsyntax(lua_State *state, int stack)
{
	const char *options[] =
	{
	// Grammars
	"basic", "extended", "awk", "grep", "egrep", "ECMA",
	// Modifiers
	"icase", "nosubs", "optimize", "collate",
	// End
	nullptr
	};
	switch (luaL_checkoption(state, stack, "ECMA", options))
	{
	case 0: return basic;
	case 1: return extended;
	case 2: return awk;
	case 3: return grep;
	case 4: return egrep;
	case 5: return ECMAScript;
	case 6: return icase;
	case 7: return nosubs;
	case 8: return optimize;
	case 9: return collate;
	}
}

static match_flag_type optmatch(lua_State *state, int stack)
{
	const char *options[] =
	{
	"default", "any", "not empty", "continuous", nullptr
	};
	switch (luaL_checkoption(state, stack, "default", options))
	{
	case 1: return match_any;
	case 0: return match_default;
	case 2: return match_not_null;
	case 3: return match_continuous;
	}
}

struct Regex
{
	typedef lux_Store<std::regex> Type;
	
	static int __new(lua_State *state)
	{
		size_t size;
		// Pattern of the regular expression
		auto string = lua_tolstring(state, 1, &size);
		// Number of optional arguments
		int top = lua_gettop(state);
		// Convert strings into flags
		syntax_option_type type = 0;
		for (int arg = 2; arg < top; ++arg)
			type |= optsyntax(state, arg);
		// Construct the regex object with given arguments
		new (state) std::regex(string, size, type);
		luaL_setmetatable(state, Type::name);
		return 1;
	}

	static int __gc(lua_State *state)
	{
		using namespace std;
		// Explicit destruction
		Type::to(state).~regex();
		return 0;
	}

	static int __tostring(lua_State *state)
	{
		// Return the internal string presentation and storage pointer
		lua_pushfstring(state, "%s: %p", Type::name, Type::test(state));
		return 1;
	}

	static int __len(lua_State *state)
	{
		unsigned n = Type::to(state).mark_count();
		lua_pushinteger(state, n);
		return 1;
	}

	static int match(lua_State *state)
	{
		auto regex = Type::to(state, 1);
		auto string = luaL_checkstring(state, 2);
		// Number of optional arguments
		int top = lua_gettop(state);
		// Convert strings to flags
		match_flag_type type = 0;
		for (int arg = 3; arg < top; ++arg)
			type |= optmatch(state, arg);
		// Call the regex match function with given flags
		bool find = std::regex_match(string, regex, type);
		lua_pushboolean(state, find);
		return 1;
	}

	static int search(lua_State *state)
	{
		auto regex = Type::to(state, 1);
		auto string = luaL_checkstring(state, 2);
		// Number of optional arguments
		int top = lua_gettop(state);
		// Convert strings to flags
		match_flag_type type = 0;
		for (int arg = 3; arg < top; ++arg)
			type |= optmatch(state, arg);
		// Call the regex match function with given flags
		bool find = std::regex_match(string, regex, type);
		lua_pushboolean(state, find);
		return 1;
	}

	static int open(lua_State *state)
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

		lua_pushliteral(state, "__tostring");
		lua_pushcfunction(state, __tostring);
		lua_settable(state, -3);

		luaL_Reg index[] =
		{
		{"match", match},
		{"search", search},
		{nullptr}
		};
		
		lua_pushliteral(state, "__index");
		luaL_newlib(state, index);
		lua_settable(state, -3);

		return 1;
	}
};

// Lua module entry point

extern "C" int luaopen_regex(lua_State *state)
{
	return Regex::open(state);
}

