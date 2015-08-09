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
 *
 * This is highly experimental.
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

	/// Extract real and imaginary parts
	static int __call(lua_State *state)
	{
		auto &data = obj(state);
		auto real = data.real();
		auto imag = data.imag();
		return lux_push(state, imag, real);
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

// Overloaded for implicit conversion from related numeric type

template <> inline
std::complex<double> lux_to(lua_State *state, int stack)
{
	typedef lux_Store<std::complex<double>> Type;

	if (lua_isnumber(state, stack))
	return lua_tonumber(state, stack);
	else
	return Type::to(state, stack);
}

// Specialize several array methods for copmlex type //////////////////////////

template <>
// The same except does not accept construction from strings
int lux_Array<std::complex<double>>::__new(lua_State *state)
{
	typedef std::complex<double> User;
	typedef lux_Store<User*> Type;
	// Array data
	size_t size;
	User *data;
	// Check constructor argument
	switch (lua_type(state, 1))
	{
	  default:
		// We only construct from these argument types
		return luaL_argerror(state, 1, "number, table");
	  case LUA_TNUMBER:
		size = lua_tointeger(state, 1);
		lux_argcheck(state, 1, 0 < size);
		data = new User [size];
		break;
	  case LUA_TTABLE:
		// Copy table contents
		size = luaL_len(state, 1);
		data = new User [size];
		lua_pushnil(state);
		for (int it = 0; lua_next(state, 1); ++it)
		{
		 if (lua_isnumber(state, 3))
		 data[it] = lua_tonumber(state, 3);
		 else
		 data[it] = lux_to<User>(state, 3);
		 lua_pop(state, 1);
		}
		break;
	};
	// Put the array on the stack
	Type::push(state, data, size);
	return 1;
}

template <>
// String conversion for printing both components properly
int lux_Array<std::complex<double>>::__tostring(lua_State *state)
{
	Type *user = Type::check(state);
	int size = abs(user->size);
	// Build up a string
	lux_Buffer buffer(state);
	// Iterate over each item in the array
	for (int item = 0; item < size; ++item)
	{
		// Add separator only after first
		if (item) buffer.addstring(", ");
		// Get real and imaginary parts
		auto data = user->data + item;
		auto real = data->real();
		auto imag = data->imag();
		// Convert imaginary part
		buffer.addchar('i');
		lux_push(state, imag);
		buffer.addvalue();
		// Convert the real part
		buffer.addstring(" + ");
		lux_push(state, real);
		buffer.addvalue();
	}
	buffer.push();
	return 1;
}

template <>
// More efficient way to compare two complex numbers for qsort and friends
int lux_Array<std::complex<double>>::compare(const void *p, const void *q)
{
	// We will call norm only 2 times instead of 4 times
	auto n = std::norm(*(const std::complex<double> *) p);
	auto m = std::norm(*(const std::complex<double> *) q);
	// Compare by distance to origin in complex plane
	return n < m ? -1 : m < n ? +1 : 0;
}

template <>
// Conversion of complex numbers to string is prohibited
int lux_Array<std::complex<double>>::puts(lua_State *state)
{
	return luaL_error(state, "Put string not supported for complex");
}

template <>
// Conversion of complex numbers from string is prohibited
int lux_Array<std::complex<double>>::gets(lua_State *state)
{
	return luaL_error(state, "Get string not supported for complex");
}

template <>
// When loading, load the numeric type and the number i too
int lux_Array<std::complex<double>>::open(lua_State *state)
{
	// Open the numeric type first
	lux_Complex<double>::open(state);
	// Next open the complex array type
	Type::name = lua_tostring(state, +1);
	if (luaL_newmetatable(state, Type::name))
	{
		luaL_Reg regs [] =
		{
		{"sort", sort},
		{"search", search},
		{"write", write},
		{"read", read},
		{"copy", copy},
		{"swap", swap},
		{"new", __new},
		{"__gc", __gc},
		{"__tostring", __tostring},
		{"__concat", __concat},
		{"__newindex", __newindex},
		{"__index", __index},
		{"__len", __len},
		{"__mul", __mul},
		{"__div", __div},
		{"__add", __add},
		{"__sub", __sub},
		{"__shl", __shl},
		{"__shr", __shr},
		{"__unm", __unm},
		{"__eq", __eq},
		{"__lt", __lt},
		{"__le", __le},
		{nullptr}
		};
		luaL_setfuncs(state, regs, 0);
		// The imaginary sqrt(-1)
		lua_pushliteral(state, "i");
		lux_push(state, std::complex<double>(0, 1));
		lua_settable(state, -3);
	}
	return 1;
}

#endif // file

