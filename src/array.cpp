/**
 * Straight-forward implementation of POD arrays with Unicode conversions.
 */

#include "lux.hpp"

extern "C" int luaopen_array(lua_State *state)
{
	lua_newtable(state);
	int tab = lua_gettop(state);
	// Common numeric types
	luaL_Reg regs[] =
	{
	{"int", lux_Array<int>::open},
	{"char", lux_Array<char>::open},
	{"short", lux_Array<short>::open},
	{"long", lux_Array<long>::open},
	{"float", lux_Array<float>::open},
	{"double", lux_Array<double>::open},
	{nullptr}
	};
	// Register and put in module table
	for (auto reg=regs; reg->name; ++reg)
	{
		// Create the metatable but do not make it global
		luaL_requiref(state, reg->name, reg->func, false);
		// Store in returned module table
		lua_setfield(state, tab, reg->name);
	}
	// Character coding
	luaL_Reg codes[] =
	{
	{"int", lux_Coder<int>::open},
	{"char", lux_Coder<char>::open},
	{"short", lux_Coder<short>::open},
	{"long", lux_Coder<long>::open},
	{nullptr}
	};
	// Add UTF encode/decode to integer types
	for (auto code=codes; code->name; ++code)
	{
		// Include the metatable name
		lua_pushstring(state, code->name);
		// Call the loader
		code->func(state);
		// Pop name & table
		lua_pop(state, 2);
	}
	// Done
	return 1;
}

