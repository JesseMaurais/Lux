#ifndef __lxcmath__
#define __lxcmath__

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

#include "lxstore.hpp"
#include "lxstack.hpp"
#include "lxarray.hpp"
#include <complex>
#include <cmath>

/// Support for complex numbers in Lua

template <class Real> struct lux_Complex
{
	// Using the standard C++ class
	using Complex = std::complex<Real>;

	// Basic storage as raw object
	typedef lux_Store<Complex> Type;

	/// Returns a reference to the stack object or not at all
	static inline Complex &obj(lua_State *state, int stack=1)
	{
		return Type::check(state, stack)->data;
	}

	/// Returns a stack value coerced to a (real) number
	static inline Real val(lua_State *state, int stack=2)
	{
		return lua_tonumber(state, stack);
	}

	/// Convert to a string for printing
	static int __tostring(lua_State *state)
	{
		auto &data = obj(state);
		auto real = data.real();
		auto imag = data.imag();
		lua_pushfstring(state, "i%f + %f", imag, real);
		return 1;
	}

	/// Product with real or complex
	static int __mul(lua_State *state)
	{
		if (lua_isnumber(state, 2))
		lux_push(state, obj(state, 1) * val(state, 2));
		else
		lux_push(state, obj(state, 1) * obj(state, 2));
		return 1;
	}

	/// Division by real or complex
	static int __div(lua_State *state)
	{
		if (lua_isnumber(state, 2))
		lux_push(state, obj(state, 1) / val(state, 2));
		else
		lux_push(state, obj(state, 1) / obj(state, 2));
		return 1;
	}

	/// Addition with real or complex
	static int __add(lua_State *state)
	{
		if (lua_isnumber(state, 2))
		lux_push(state, obj(state, 1) + val(state, 2));
		else
		lux_push(state, obj(state, 1) + obj(state, 2));
		return 1;
	}

	/// Subraction by real or complex
	static int __sub(lua_State *state)
	{
		if (lua_isnumber(state, 2))
		lux_push(state, obj(state, 1) - val(state, 2));
		else
		lux_push(state, obj(state, 1) - obj(state, 2));
		return 1;
	}

	/// Unary minus (or sign change)
	static int __unm(lua_State *state)
	{
		lux_push(state, - obj(state, 1));
		return 1;
	}

	/// Equality of two complexes
	static int __eq(lua_State *state)
	{
		lua_pushboolean(state, obj(state, 1) == obj(state, 2));
		return 1;
	}

	/// Closer to the origin in the plane
	static int __lt(lua_State *state)
	{
		lua_pushboolean(state, obj(state, 1) < obj(state, 2));
		return 1;
	}

	/// At least as close to the origin
	static int __le(lua_State *state)
	{
		lua_pushboolean(state, obj(state, 1) <= obj(state, 2));
		return 1;
	}

	/// Loader compatible with luaL_requiref
	static int open(lua_State *state)
	{
		// Create the array class first
		lux_Array<Complex>::open(state);
		// Create base complex type second
		luaL_newmetatable(state, Type::name);
		luaL_Reg regs[] =
		{
		{"__tostring", __tostring},
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
		// The imaginary sqrt(-1)
		lua_pushliteral(state, "i");
		lux_push(state, Complex(0.0, 1.0));
		lua_settable(state, -3);
		return 1;
	}

	// Comparison that is compatible with qsort and friends

	static inline bool compare(const void *p, const void *q)
	{
		// We call norm 2 instead of 4 times
		auto n = std::norm(*(const Complex *) p);
		auto m = std::norm(*(const Complex *) q);
		return n < m ? -1 : m < n ? +1 : 0;
	}

	// Avoid a bug (?) in luaL_addvalue

	static inline int tostring(lua_State *state)
	{
		typedef lux_Array<Complex> Array;
		auto user = Array::Type::check(state);
		int size = abs(user->size);
		// Build up a string
		lux_Buffer buffer(state);
		// Iterate over each item in the array
		for (int item = 0; item < size; ++item)
		{
			Real real = user->data[item].real();
			Real imag = user->data[item].imag();
			// Add separator only after first
			if (item) buffer.addstring(", ");
			buffer.addchar('i');
			lux_push(state, imag);
			buffer.addvalue();
			buffer.addstring(" + ");
			lux_push(state, real);
			buffer.addvalue();
		}
		buffer.push();
		return 1;
	}
};

// Relations are determined relative to the origin on the complex plane

template <class Real> inline
bool operator < (const std::complex<Real> &c, const std::complex<Real> &d)
{
	return std::norm(c) < std::norm(d);
}

template <class Real> inline
bool operator <= (const std::complex<Real> &c, const std::complex<Real> &d)
{
	return std::norm(c) <= std::norm(d);
}

// Have arrays of complex numbers use a more efficient compare in qsort

template <>
int lux_Array<std::complex<float>>::compare(const void *p, const void *q)
{
	return lux_Complex<float>::compare(p, q);
}
template <>
int lux_Array<std::complex<double>>::compare(const void *p, const void *q)
{
	return lux_Complex<double>::compare(p, q);
}
template <>
int lux_Array<std::complex<long double>>::compare(const void *p, const void *q)
{
	return lux_Complex<long double>::compare(p, q);
}

// Arrays cannot convert the complex properly because of luaL_Buffer

template <>
int lux_Array<std::complex<float>>::__tostring(lua_State *state)
{
	return lux_Complex<float>::tostring(state);
}
template <>
int lux_Array<std::complex<double>>::__tostring(lua_State *state)
{
	return lux_Complex<double>::tostring(state);
}
template <>
int lux_Array<std::complex<long double>>::__tostring(lua_State *state)
{
	return lux_Complex<long double>::tostring(state);
}

// Strange as it seems, we need to convert characters to put them in arrays

template <>
int lux_Chars::from<std::complex<float>>
	(char *dst, const std::complex<float> *src, size_t len)
{
	char32_t buf[len];
	for (int n = 0; n < len; ++n) buf[n] = src[n].real();
	return fromstring(dst, buf, len);
}
template <>
int lux_Chars::to<std::complex<float>>
	(std::complex<float> *dst, const char *src, size_t len)
{
	char32_t buf[len];
	int sz = tostring(buf, src, len);
	for (int n = 0; n < sz; ++n) dst[n].real(buf[n]);
	return sz;
}

template <>
int lux_Chars::from<std::complex<double>>
	(char *dst, const std::complex<double> *src, size_t len)
{
	char32_t buf[len];
	for (int n = 0; n < len; ++n) buf[n] = src[n].real();
	return fromstring(dst, buf, len);
}
template <>
int lux_Chars::to<std::complex<double>>
	(std::complex<double> *dst, const char *src, size_t len)
{
	char32_t buf[len];
	int sz = tostring(buf, src, len);
	for (int n = 0; n < sz; ++n) dst[n].real(buf[n]);
	return sz;
}

template <>
int lux_Chars::from<std::complex<long double>>
	(char *dst, const std::complex<long double> *src, size_t len)
{
	char32_t buf[len];
	for (int n = 0; n < len; ++n) buf[n] = src[n].real();
	return fromstring(dst, buf, len);
}
template <>
int lux_Chars::to<std::complex<long double>>
	(std::complex<long double> *dst, const char *src, size_t len)
{
	char32_t buf[len];
	int sz = tostring(buf, src, len);
	for (int n = 0; n < sz; ++n) dst[n].real(buf[n]);
	return sz;
}

#endif // file

