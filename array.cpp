#include "lux.hpp"

extern "C" int luaopen_array(lua_State *state)
{
	luaL_Reg regs [] =
	{
	{"int", lux_Array<int>::open},
	{"char", lux_Array<char>::open},
	{"short", lux_Array<short>::open},
	{"long", lux_Array<long>::open},
	{"float", lux_Array<float>::open},
	{"double", lux_Array<double>::open},
	{nullptr, nullptr}
	};
	for (auto r=regs; r->name; ++r)
	{
	 luaL_requiref(state, r->name, r->func, true);
	 lua_pop(state, 1);
	}
	return 0;
}

