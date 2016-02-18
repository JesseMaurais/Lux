#include "lux.hpp"

// Straight-forward implementation of POD arrays with Unicode conversions

extern "C" int luaopen_array(lua_State *state)
{
	// Common numeric functions
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
	for (auto reg=regs; reg->name; ++reg)
	{
	 luaL_requiref(state, reg->name, reg->func, true);
	 lua_pop(state, 1);
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
	for (auto code=codes; code->name; ++code)
	{
	 lua_pushstring(state, code->name);
	 code->func(state);
	 lua_pop(state, 1);
	}
	// Done
	return 0;
}

