#ifndef __lxqmath__
#define __lxqmath__

#include "lxstore.hpp"
#include "lxarray.hpp"

template <class Integer> struct lux_Rational
{
	Integer num, den;

	lux_Rational(Integer num=0, Integer den=1)
	{
		this->num = num;
		this->den = den;
	}

	void operator = (const lux_Rational &that)
	{
		this->num = that.num;
		this->den = that.den;
	}

	void operator = (const Integer that)
	{
		num = that;
		den = 1;
	}

	bool operator < (const lux_Rational &that)
	{
		Integer n = num*that.den;
		Integer m = den*that.num;
		return n < m;
	}

	bool operator <= (const lux_Rational &that)
	{
		Integer n = num*that.den;
		Integer m = den*that.num;
		return n <= m;
	}

	typedef lux_Rational<Integer> User;
	typedef lux_Store<User> Type;

	static int make(lua_State *state)
	{
		User Q;
		Q.num = luaL_optinteger(state, 1, 0);
		Q.den = luaL_optinteger(state, 2, 1);
		lux_push(state, Q);
		return 1;
	}

	static int __tostring(lua_State *state)
	{
		User Q = Type::to(state);
		lua_pushfstring(state, "%d/%d", Q.num, Q.den);
		return 1;
	}

	static int __add(lua_State *state)
	{
		User Q = lux_to<User>(state, 1);
		User R = lux_to<User>(state, 2);
		auto num = Q.num*R.den + R.num*Q.den;
		auto den = Q.den*R.den;
		lux_push(state, User(num, den));
		return 1;
	}

	static int __sub(lua_State *state)
	{
		User Q = lux_to<User>(state, 1);
		User R = lux_to<User>(state, 2);
		auto num = Q.num*R.den - R.num*Q.den;
		auto den = Q.den*R.den;
		lux_push(state, User(num, den));
		return 1;
	}

	static int __mul(lua_State *state)
	{
		User Q = lux_to<User>(state, 1);
		User R = lux_to<User>(state, 2);
		auto num = Q.num*R.num;
		auto den = Q.den*R.den;
		lux_push(state, User(num, den));
		return 1;
	}

	static int __div(lua_State *state)
	{
		User Q = lux_to<User>(state, 1);
		User R = lux_to<User>(state, 2);
		auto num = Q.num*R.den;
		auto den = Q.den*R.num;
		lux_push(state, User(num, den));
		return 1;
	}

	static int __eq(lua_State *state)
	{
		User Q = lux_to<User>(state, 1);
		User R = lux_to<User>(state, 2);
		auto n = Q.num * R.den;
		auto m = Q.den * R.num;
		lua_pushboolean(state, n == m);
		return 0;
	}

	static int __lt(lua_State *state)
	{
		User Q = Type::to(state, 1);
		User R = Type::to(state, 2);
		auto n = Q.num * R.den;
		auto m = Q.den * R.num;
		lua_pushboolean(state, n < m);
		return 0;
	}

	static int __le(lua_State *state)
	{
		User Q = Type::to(state, 1);
		User R = Type::to(state, 2);
		auto n = Q.num * R.den;
		auto m = Q.den * R.num;
		lua_pushboolean(state, n <= m);
		return 0;
	}

	static int open(lua_State *state)
	{
		luaL_newmetatable(state, Type::name);
		luaL_Reg regs[] =
		{
		{"__tostring", __tostring},
		{"__add", __add},
		{"__sub", __sub},
		{"__mul", __mul},
		{"__div", __div},
		{"__eq", __eq},
		{"__lt", __lt},
		{"__le", __le},
		{nullptr}
		};
		luaL_setfuncs(state, regs, 0);
		return 1;
	}
};


template <> inline
lux_Rational<long> lux_to<lux_Rational<long>>(lua_State *state, int stack)
{
	typedef lux_Store<lux_Rational<long>> Type;
	if (lua_isinteger(state, stack))
	return lua_tointeger(state, stack);
	else
	return Type::to(state, stack);
}


// Specialized array functions for the rational type //////////////////////////


template <>
// The same except does not accept construction from strings
int lux_Array<lux_Rational<long>>::__new(lua_State *state)
{
	typedef lux_Rational<long> User;
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
		 data[it] = lua_tointeger(state, 3);
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
int lux_Array<lux_Rational<long>>::__tostring(lua_State *state)
{
	Type *user = Type::check(state);
	int size = abs(user->size);
	// Build up a string
	lux_Buffer buffer(state);
	// Iterate over each item in the array
	for (int item = 0; item < size; ++item)
	{
		auto &ratio = user->data[item];
		// Add separator only after first
		if (item) buffer.addstring(", ");
		// Convert numerator and denominator
		lua_pushinteger(state, ratio.num);
		buffer.addvalue();
		buffer.addchar('/');
		lua_pushinteger(state, ratio.den);
		buffer.addvalue();
	}
	buffer.push();
	return 1;
}

template <>
// More efficient way to compare two rational numbers for qsort and friends
int lux_Array<lux_Rational<long>>::compare(const void *p, const void *q)
{
	auto Q = (const lux_Rational<long> *) p;
	auto R = (const lux_Rational<long> *) q;
	long n = Q->num * R->den;
	long m = Q->den * R->num;
	return n < m ? -1 : m < n ? +1 : 0;
}

template <>
// Conversion of complex numbers to string is prohibited
int lux_Array<lux_Rational<long>>::puts(lua_State *state)
{
	return luaL_error(state, "Put string not supported for complex");
}

template <>
// Conversion of complex numbers to string is prohibited
int lux_Array<lux_Rational<long>>::gets(lua_State *state)
{
	return luaL_error(state, "Get string not supported for complex");
}

template <>
// When loading, load the numeric type and the number i too
int lux_Array<lux_Rational<long>>::open(lua_State *state)
{
	// Open the numeric type first
	lux_Rational<long>::open(state);
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
		{"make", lux_Rational<long>::make},
		{"new", __new},
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
		{"__gc", __gc},
		{nullptr}
		};
		luaL_setfuncs(state, regs, 0);
	}
	return 1;
}

#endif // file

