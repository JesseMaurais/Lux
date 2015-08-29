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
	typedef lux_Store<Number*> Type;

	Number *from, *to;
	size_t size;
	int results;

	Operations(lua_State *state)
	{
		// Source array to read arguments
		Type *user = Type::check(state, 1);
		// Genuine array size
		size = abs(user->size);
		// Ensure this is not a pointer
		lux_argcheck(state, 1, 0 < size);
		// Check optional target array
		to = Type::opt(state, 2);
		if (!to)
		{
			to = new Number [size];
			Type::push(state, to, size);
			results = 1;
		}
		else results = 0;
		from = user->data;
	}

	// Trigonometric functions

	static int cos(lua_State *state)
	{
		Operations args(state);
		#pragma omp parallel for
		for (int item = 0; item < args.size; ++item)
			args.to[item] = std::cos(args.from[item]);
		return args.results;
	}

	static int sin(lua_State *state)
	{
		Operations args(state);
		#pragma omp parallel for
		for (int item = 0; item < args.size; ++item)
			args.to[item] = std::sin(args.from[item]);
		return args.results;
	}

	static int tan(lua_State *state)
	{
		Operations args(state);
		#pragma omp parallel for
		for (int item = 0; item < args.size; ++item)
			args.to[item] = std::tan(args.from[item]);
		return args.results;
	}

	static int acos(lua_State *state)
	{
		Operations args(state);
		#pragma omp parallel for
		for (int item = 0; item < args.size; ++item)
			args.to[item] = std::acos(args.from[item]);
		return args.results;
	}

	static int asin(lua_State *state)
	{
		Operations args(state);
		#pragma omp parallel for
		for (int item = 0; item < args.size; ++item)
			args.to[item] = std::acos(args.from[item]);
		return args.results;
	}

	static int atan(lua_State *state)
	{
		Operations args(state);
		#pragma omp parallel for
		for (int item = 0; item < args.size; ++item)
			args.to[item] = std::atan(args.from[item]);
		return args.results;
	}

	// Hyperbolic functions

	static int cosh(lua_State *state)
	{
		Operations args(state);
		#pragma omp parallel for
		for (int item = 0; item < args.size; ++item)
			args.to[item] = std::cosh(args.from[item]);
		return args.results;
	}

	static int sinh(lua_State *state)
	{
		Operations args(state);
		#pragma omp parallel for
		for (int item = 0; item < args.size; ++item)
			args.to[item] = std::sinh(args.from[item]);
		return args.results;
	}

	static int tanh(lua_State *state)
	{
		Operations args(state);
		#pragma omp parallel for
		for (int item = 0; item < args.size; ++item)
			args.to[item] = std::tanh(args.from[item]);
		return args.results;
	}

	static int acosh(lua_State *state)
	{
		Operations args(state);
		#pragma omp parallel for
		for (int item = 0; item < args.size; ++item)
			args.to[item] = std::acosh(args.from[item]);
		return args.results;
	}

	static int asinh(lua_State *state)
	{
		Operations args(state);
		#pragma omp parallel for
		for (int item = 0; item < args.size; ++item)
			args.to[item] = std::asinh(args.from[item]);
		return args.results;
	}

	static int atanh(lua_State *state)
	{
		Operations args(state);
		#pragma omp parallel for
		for (int item = 0; item < args.size; ++item)
			args.to[item] = std::atanh(args.from[item]);
		return args.results;
	}

	// Exponential and logarithmic functions	

	static int exp(lua_State *state)
	{
		Operations args(state);
		#pragma omp parallel for
		for (int item = 0; item < args.size; ++item)
			args.to[item] = std::exp(args.from[item]);
		return args.results;
	}

	static int exp2(lua_State *state)
	{
		Operations args(state);
		#pragma omp parallel for
		for (int item = 0; item < args.size; ++item)
			args.to[item] = std::exp2(args.from[item]);
		return args.results;
	}

	static int log(lua_State *state)
	{
		Operations args(state);
		#pragma omp parallel for
		for (int item = 0; item < args.size; ++item)
			args.to[item] = std::log(args.from[item]);
		return args.results;
	}

	static int log2(lua_State *state)
	{
		Operations args(state);
		#pragma omp parallel for
		for (int item = 0; item < args.size; ++item)
			args.to[item] = std::log2(args.from[item]);
		return args.results;
	}

	static int log10(lua_State *state)
	{
		Operations args(state);
		#pragma omp parallel for
		for (int item = 0; item < args.size; ++item)
			args.to[item] = std::log10(args.from[item]);
		return args.results;
	}

	// Misc.

	static int erf(lua_State *state)
	{
		Operations args(state);
		#pragma omp parallel for
		for (int item = 0; item < args.size; ++item)
			args.to[item] = std::erf(args.from[item]);
		return args.results;
	}

	static int gamma(lua_State *state)
	{
		Operations args(state);
		#pragma omp parallel for
		for (int item = 0; item < args.size; ++item)
			args.to[item] = std::tgamma(args.from[item]);
		return args.results;
	}

	static int lgamma(lua_State *state)
	{
		Operations args(state);
		#pragma omp parallel for
		for (int item = 0; item < args.size; ++item)
			args.to[item] = std::lgamma(args.from[item]);
		return args.results;
	}

	static int open(lua_State *state)
	{
		luaL_Reg index[] =
		{
		// trigonometric
		{"cos", cos},
		{"sin", sin},
		{"tan", tan},
		{"acos", acos},
		{"asin", asin},
		{"atan", atan},
		// hyperbolic
		{"cosh", cosh},
		{"sinh", sinh},
		{"tanh", tanh},
		{"acosh", acosh},
		{"asinh", asinh},
		{"atanh", atanh},
		// exponential
		{"exp", exp},
		{"exp2", exp2},
		{"log", log},
		{"log2", log2},
		{"log10", log10},
		// misc.
		{"erf", erf},
		{"gamma", gamma},
		{"lgamma", lgamma},
		// end
		{nullptr}
		};
		luaL_setfuncs(state, index, 0);
		return 0;
	}
};

// Lua module entry point

extern "C" int luaopen_cmath(lua_State *state)
{
	luaL_Reg regs[] =
	{
	{"float", Operations<float>::open},
	{"double", Operations<double>::open},
	{"complex", Operations<complex<float>>::open},
	{"complexd", Operations<complex<double>>::open},
	{nullptr}
	};
	for (auto r=regs; r->name; ++r)
	{
		if (luaL_getmetatable(state, r->name))
		{
			r->func(state);
			lua_pop(state, 1);
		}
	}
	return 0;
}

