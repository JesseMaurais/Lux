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
	{lux_Array<int>::Store::name, lux_Array<int>::open},
	{lux_Array<char>::Store::name, lux_Array<char>::open},
	{lux_Array<short>::Store::name, lux_Array<short>::open},
	{lux_Array<long>::Store::name, lux_Array<long>::open},
	{lux_Array<float>::Store::name, lux_Array<float>::open},
	{lux_Array<double>::Store::name, lux_Array<double>::open},
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
	{lux_Array<int>::Store::name, lux_Coder<int>::open},
	{lux_Array<char>::Store::name, lux_Coder<char>::open},
	{lux_Array<short>::Store::name, lux_Coder<short>::open},
	{lux_Array<long>::Store::name, lux_Coder<long>::open},
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

