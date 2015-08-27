#include "lux.hpp"
#include <complex>
#include <cmath>

template <class real> using complex = std::complex<real>;

// Use if enabled
#ifdef _OPENMP
#include <omp.h>
#endif

template <class Number> struct Operations
{
	typedef lux_Array<Number> Array;
	typedef lux_Store<Number*> Type;

	static int sin(lua_State *state)
	{
		// Target array to write results
		auto to = Type::check(state, 1);
		int to_size = abs(to->size);
		// Source array to read parameters
		auto from = Type::check(state, 2);
		int from_size = abs(from->size);
		// Ensure that this is an array
		lux_argcheck(state, 1, 0 < to_size);
		// Ensure that the arrays are the same size
		lux_argcheck(state, 2, from_size == to_size);
		// Use OpenMP if enabled
		#pragma omp parallel for
		for (int item = 0; item < to_size; ++item)
			to->data[item] = std::sin(from->data[item]);
		// No result
		return 0;
	}

	static int open(lua_State *state)
	{
		auto name = lua_tostring(state, 1);
		if (!luaL_getmetatable(state, name))
		{
			return luaL_error(state, "require 'array'");
		}
		luaL_Reg regs[] =
		{
		{"sin", sin},
		{nullptr}
		};
		luaL_setfuncs(state, regs, 0);
		return 1;
	}
};

// Lua module entry point

extern "C" int luaopen_cmath(lua_State *state)
{
	luaL_Reg regs[] =
	{
	{"float", Operations<float>::open},
	{"double", Operations<double>::open},
//	{"complex", Operations<complex<float>>::open},
//	{"complexd", Operations<complex<double>>::open},
	{nullptr}
	};
}

