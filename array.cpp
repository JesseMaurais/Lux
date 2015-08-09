#include "lux.hpp"

using complex = std::complex<double>;
using rational = lux_Rational<long>;

// Straight-forward implementation of POD arrays

extern "C" int luaopen_array(lua_State *state)
{
	lua_newtable(state);
	luaL_Reg regs [] =
	{
	{"int", lux_Array<int>::open},
	{"char", lux_Array<char>::open},
	{"short", lux_Array<short>::open},
	{"long", lux_Array<long>::open},
	{"float", lux_Array<float>::open},
	{"double", lux_Array<double>::open},
	{"complex", lux_Array<complex>::open},
	{"rational", lux_Array<rational>::open},
	{nullptr}
	};
	for (auto reg=regs; reg->name; ++reg)
	{
	 luaL_requiref(state, reg->name, reg->func, true);
	 lua_setfield(state, -2, reg->name);
	}
	return 1;
}

