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
	for (auto r=regs; r->name; ++r)
	{
	 luaL_requiref(state, r->name, r->func, true);
	 lua_pop(state, 1);
	}
	// UTF-8 specific functions
	luaL_Reg code[] =
	{
	{"int", lux_Coder<int>::open},
	{"char", lux_Coder<char>::open},
	{"short", lux_Coder<short>::open},
	{"long", lux_Coder<long>::open},
	{nullptr}
	};
	for (auto r=code; r->name; ++r)
	{
	 lua_pushstring(state, r->name);
	 r->func(state); // adds methods
	 lua_pop(state, 1); // name
	}
	return 0;
}

