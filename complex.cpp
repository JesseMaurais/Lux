/**
 * Support for complex numbers with all the usual operations defined in a Lua
 * metatable. The implementation is based on the standard C++ complex classes
 * rather than the C builtin types. Any code that uses these same classes can
 * easily interface with complex numbers written in Lua code through the type
 * system. Additional operations are defined so that these classes can become
 * elements in the array classes as well. For example, both the less-than and
 * the less-than-or-equal operators are defined for complex numbers, in which
 * their order are determined by their relative distance to the origin in the
 * complex plane. This allows sorting arrays of complex numbers.
 */

#include "lux.hpp"
#include <complex>
#include <cmath>

template <class real> using complex = std::complex<real>;

// Relations are determined relative to the origin on the complex plane

template <class real> inline
bool operator < (const complex<real> &c, const complex<real> &d)
{
	return std::norm(c) < std::norm(d);
}

template <class real> inline
bool operator <= (const complex<real> &c, const complex<real> &d)
{
	return std::norm(c) <= std::norm(d);
}

// Overloaded for implicit conversion from related numeric type

template <> inline
complex<float> lux_to<complex<float>>(lua_State *state, int stack)
{
	if (lua_isnumber(state, stack)) return lua_tonumber(state, stack);
	return lux_Store<complex<float>>::to(state, stack);
}

template <> inline
complex<double> lux_to<complex<double>>(lua_State *state, int stack)
{
	if (lua_isnumber(state, stack)) return lua_tonumber(state, stack);
	return lux_Store<complex<double>>::to(state, stack);
}

template <> inline
complex<long double> lux_to<complex<long double>>(lua_State *state, int stack)
{
	if (lua_isnumber(state, stack)) return lua_tonumber(state, stack);
	return lux_Store<complex<long double>>::to(state, stack);
}

// Storage class to define the methods of the complex number type

template <class Real> struct lux_Complex
{
	// Using the standard C++ class
	using Complex = complex<Real>;

	// Basic storage as raw object
	typedef lux_Store<Complex> Type;

	// Returns a reference to the stack object or not at all
	static inline Complex &obj(lua_State *state, int stack=1)
	{
		return Type::check(state, stack)->data;
	}

	// Returns a stack value converted to a (real) number
	static inline Real val(lua_State *state, int stack=2)
	{
		return lua_tonumber(state, stack);
	}

	// Convert to a string for printing
	static int __tostring(lua_State *state)
	{
		auto &data = obj(state);
		auto real = data.real();
		auto imag = data.imag();
		lua_pushfstring(state, "i%f + %f", imag, real);
		return 1;
	}

	// Extract real and imaginary parts
	static int __call(lua_State *state)
	{
		auto &data = obj(state);
		auto real = data.real();
		auto imag = data.imag();
		return lux_push(state, imag, real);
	}

	// Product with real or complex
	static int __mul(lua_State *state)
	{
		if (lua_isnumber(state, 2))
		lux_push(state, obj(state, 1) * val(state, 2));
		else
		lux_push(state, obj(state, 1) * obj(state, 2));
		return 1;
	}

	// Division by real or complex
	static int __div(lua_State *state)
	{
		if (lua_isnumber(state, 2))
		lux_push(state, obj(state, 1) / val(state, 2));
		else
		lux_push(state, obj(state, 1) / obj(state, 2));
		return 1;
	}

	// Addition with real or complex
	static int __add(lua_State *state)
	{
		if (lua_isnumber(state, 2))
		lux_push(state, obj(state, 1) + val(state, 2));
		else
		lux_push(state, obj(state, 1) + obj(state, 2));
		return 1;
	}

	// Subraction by real or complex
	static int __sub(lua_State *state)
	{
		if (lua_isnumber(state, 2))
		lux_push(state, obj(state, 1) - val(state, 2));
		else
		lux_push(state, obj(state, 1) - obj(state, 2));
		return 1;
	}

	// Unary minus (or sign change)
	static int __unm(lua_State *state)
	{
		lux_push(state, - obj(state, 1));
		return 1;
	}

	// Equality of two complexes
	static int __eq(lua_State *state)
	{
		lua_pushboolean(state, obj(state, 1) == obj(state, 2));
		return 1;
	}

	// Closer to the origin in the plane
	static int __lt(lua_State *state)
	{
		lua_pushboolean(state, obj(state, 1) < obj(state, 2));
		return 1;
	}

	// At least as close to the origin
	static int __le(lua_State *state)
	{
		lua_pushboolean(state, obj(state, 1) <= obj(state, 2));
		return 1;
	}

	// Loader compatible with luaL_requiref
	static int open(lua_State *state)
	{
		luaL_newmetatable(state, Type::name);
		luaL_Reg regs[] =
		{
		{"__tostring", __tostring},
		{"__call", __call},
		{"__add", __add},
 		{"__sub", __sub},
		{"__mul", __mul},
		{"__div", __div},
		{"__unm", __unm},
		{"__eq", __eq},
		{"__lt", __lt},
		{"__le", __le},
		{nullptr}
		};
		luaL_setfuncs(state, regs, 0);
		lua_pop(state, 1);
		// Open the complex array type
		lux_Array<Complex>::open(state);
		// The imaginary sqrt(-1)
		lua_pushliteral(state, "i");
		lux_push(state, Complex(0, 1));
		lua_settable(state, -3);
		return 1;
 	}

	// Custom element compare for overloading arrays
	static int compare(const void *p, const void *q)
	{
		// Call norm only 2 times instead of 4 
		auto n = std::norm(*(const Complex *) p);
		auto m = std::norm(*(const Complex *) q);
		// Compare their magnitudes squared
		return n < m ? -1 : m < n ? +1 : 0;
	}
};

// Overload arrays to use optimized complex number comparison

template <>
int lux_Array<complex<float>>::compare(const void *p, const void *q)
{
	return lux_Complex<float>::compare(p, q);
}

template <>
int lux_Array<complex<double>>::compare(const void *p, const void *q)
{
	return lux_Complex<double>::compare(p, q);
}

template <>
int lux_Array<complex<long double>>::compare(const void *p, const void *q)
{
	return lux_Complex<long double>::compare(p, q);
}

// Overload arrays to prohibit UTF-8 string conversions

template <>
int lux_Array<complex<float>>::puts(lua_State *state)
{
	return luaL_error(state, "Put string unsupported for complex");
}

template <>
int lux_Array<complex<double>>::puts(lua_State *state)
{
	return luaL_error(state, "Put string unsupported for complex");
}

template <>
int lux_Array<complex<long double>>::puts(lua_State *state)
{
	return luaL_error(state, "Put string unsupported for complex");
}

template <>
int lux_Array<complex<float>>::gets(lua_State *state)
{
	return luaL_error(state, "Get string unsupported for complex");
}

template <>
int lux_Array<complex<double>>::gets(lua_State *state)
{
	return luaL_error(state, "Get string unsupported for complex");
}

template <>
int lux_Array<complex<long double>>::gets(lua_State *state)
{
	return luaL_error(state, "Get string unsupported for complex");
}

template <>
int lux_Array<complex<float>>::fromstring(lua_State *state)
{
	return luaL_error(state, "UTF string unsupported for complex");
}

template <>
int lux_Array<complex<double>>::fromstring(lua_State *state)
{
	return luaL_error(state, "UTF string unsupported for complex");
}

template <>
int lux_Array<complex<long double>>::fromstring(lua_State *state)
{
	return luaL_error(state, "UTF string unsupported for complex");
}

// Lua module entry point

extern "C" int luaopen_complex(lua_State *state)
{
	luaL_Reg regs[] =
	{
	{"complex", lux_Complex<float>::open},
	{"copmlexd", lux_Complex<double>::open},
	{nullptr}
	};
	for (auto r=regs; r->name; ++r)
	{
	 luaL_requiref(state, r->name, r->func, true);
	 lua_pop(state, 1);
	}
	return 0;
}

