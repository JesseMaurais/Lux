#include "lux.hpp"
#include <random>

// Base functions for random number generating classes

template <class Base> struct Random
{
	typedef lux_Store<Base> Type;

	// Create the random number generator
	static int __new(lua_State *state)
	{
		new (state) Type;
		luaL_setmetatable(state, Type::name);
		return 1;
	}

	// String conversion for printing
	static int __tostring(lua_State *state)
	{
		lua_pushfstring(state, "random.%s", Type::name);
		return 1;
	}

	// Garbage collection callback
	static int __gc(lua_State *state)
	{
		Type *user = Type::check(state);
		user->data.~Base();
		return 0;
	}

	// Generate a sequence of random numbers
	static int __call(lua_State *state)
	{
		auto user = Type::check(state, 1);
		// If many, store them in a table
		size_t size = luaL_optinteger(state, 2, 0);
		if (size)
		{
			// Put into a table
			lua_newtable(state);
			// Generate random numbers in the table
			for (int item = 0; item < size; item++)
			{
				lua_pushinteger(state, item);
				auto value = user->data();
				lux_push(state, value);
				lua_settable(state, -3);
			}
		}
		else
		{
			// Generate just one number
			auto value = user->data();
			lux_push(state, value);
		}
		return 1;
	}

	// Similar to above but following a given distribution
	template <class Distribution>
	static int call(lua_State *state, Distribution &distribution)
	{
		Type *user = Type::check(state, 1);
		// If many, store them in a table
		size_t size = luaL_optinteger(state, 2, 0);
		if (size)
		{
			// Put into a table
			lua_newtable(state);
			// Generate random numbers in the table
			for (int item = 0; item < size; item++)
			{
				lua_pushinteger(state, item);
				auto value = distribution(user->data);
				lux_push(state, value);
				lua_settable(state, -3);
			}
		}
		else
		{
			// Generate just one number
			auto value = distribution(user->data);
			lux_push(state, value);
		}
		return 1;
	}

	static int uniform(lua_State *state)
	{
		auto a = luaL_optnumber(state, 3, 0.0);
		auto b = luaL_optnumber(state, 4, 1.0);
		luaL_argcheck(state, a < b, 4, "a < b");

		std::uniform_real_distribution<lua_Number> distribution(a, b);
		return call(state, distribution);
	}

	static int bernoulli(lua_State *state)
	{
		auto p = luaL_optnumber(state, 3, 0.5);
		luaL_argcheck(state, 0 < p && p < 1, 3, "0 < p < 1");

		std::bernoulli_distribution distribution(p);
		return call(state, distribution);
	}

	static int binomial(lua_State *state)
	{
		auto n = luaL_optinteger(state, 3, 1);
		luaL_argcheck(state, n > 0, 3, "n > 0");
		auto p = luaL_optnumber(state, 4, 0.5);
		luaL_argcheck(state, 0 < p && p < 1, 3, "0 < p < 1");

		std::binomial_distribution<lua_Integer> distribution(n, p);
		return call(state, distribution);
	}

	static int geometric(lua_State *state)
	{
		auto p = luaL_optnumber(state, 3, 0.5);
		luaL_argcheck(state, 0 < p && p < 1, 3, "0 < p < 1");

		std::geometric_distribution<lua_Integer> distribution(p);
		return call(state, distribution);
	}

	static int negative_binomial(lua_State *state)
	{
		auto k = luaL_optinteger(state, 3, 1);
		luaL_argcheck(state, k > 0, 3, "k > 0");
		auto p = luaL_optnumber(state, 4, 0.5);
		luaL_argcheck(state, 0 < p && p < 1, 4, "0 < p < 1");

		using namespace std;
		negative_binomial_distribution<lua_Integer> distribution(k, p);
		return call(state, distribution);
	}

	static int poisson(lua_State *state)
	{
		auto mu = luaL_optinteger(state, 3, 1.0);
		luaL_argcheck(state, mu > 0, 3, "mu > 0");

		std::poisson_distribution<lua_Integer> distribution(mu);
		return call(state, distribution);
	}

	static int exponential(lua_State *state)
	{
		auto lambda = luaL_optnumber(state, 3, 1.0);
		luaL_argcheck(state, lambda > 0, 3, "lambda > 0");

		std::exponential_distribution<lua_Number> distribution(lambda);
		return call(state, distribution);
	}

	static int gamma(lua_State *state)
	{
		auto alpha = luaL_optnumber(state, 3, 1.0);
		luaL_argcheck(state, alpha > 0, 3, "alpha > 0");
		auto beta = luaL_optnumber(state, 4, 1.0);
		luaL_argcheck(state, beta > 0, 4, "beta > 0");

		std::gamma_distribution<lua_Number> distribution(alpha, beta);
		return call(state, distribution);
	}

	static int weibull(lua_State *state)
	{
		auto a = luaL_optnumber(state, 3, 1.0);
		luaL_argcheck(state, a > 0, 3, "a > 0");
		auto b = luaL_optnumber(state, 4, 1.0);
		luaL_argcheck(state, b > 0, 4, "b > 0");

		std::weibull_distribution<lua_Number> distribution(a, b);
		return call(state, distribution);
	}

	static int extreme_value(lua_State *state)
	{
		auto a = luaL_optnumber(state, 3, 0.0);
		auto b = luaL_optnumber(state, 4, 1.0);

		std::extreme_value_distribution<lua_Number> distribution(a, b);
		return call(state, distribution);
	}

	static int normal(lua_State *state)
	{
		auto mu = luaL_optnumber(state, 3, 0.0);
		auto sigma = luaL_optnumber(state, 4, 1.0);
		luaL_argcheck(state, sigma > 0, 4, "sigma > 0");

		std::normal_distribution<lua_Number> distribution(mu, sigma);
		return call(state, distribution);
	}

	static int lognormal(lua_State *state)
	{
		auto mu = luaL_optnumber(state, 3, 0.0);
		auto sigma = luaL_optnumber(state, 4, 1.0);
		luaL_argcheck(state, sigma > 0, 4, "sigma > 0");

		std::lognormal_distribution<lua_Number> distribution(mu, sigma);
		return call(state, distribution);
	}

	static int chi_squared(lua_State *state)
	{
		auto n = luaL_optinteger(state, 3, 1);
		luaL_argcheck(state, n > 0, 3, "n > 0");

		std::chi_squared_distribution<lua_Number> distribution(n);
		return call(state, distribution);
	}

	static int cauchy(lua_State *state)
	{
		auto pos = luaL_optnumber(state, 3, 0.0);
		auto scale = luaL_optnumber(state, 4, 1.0);
		luaL_argcheck(state, scale > 0, 4, "scale > 0");

		std::cauchy_distribution<lua_Number> distribution(pos, scale);
		return call(state, distribution);
	}

	static int fisher(lua_State *state)
	{
		auto m = luaL_optinteger(state, 3, 1);
		luaL_argcheck(state, m > 0, 3, "m > 0");
		auto n = luaL_optinteger(state, 4, 1);
		luaL_argcheck(state, n > 0, 4, "n > 0");

		std::fisher_f_distribution<lua_Number> distribution(m, n);
		return call(state, distribution);
	}

	static int student(lua_State *state)
	{
		auto n = luaL_optinteger(state, 3, 1);
		luaL_argcheck(state, n > 0, 3, "n > 0");

		std::student_t_distribution<lua_Number> distribution(n);
		return call(state, distribution);
	}

	// Device/Engine's minimum value
	static int min(lua_State *state)
	{
		Type *user = Type::check(state);
		auto value = user->data.min();
		lux_push(state, value);
		return 1;
	}

	// Device/Engine's maximum value
	static int max(lua_State *state)
	{
		Type *user = Type::check(state);
		auto value = user->data.max();
		lux_push(state, value);
		return 1;
	}

	// Create and fill a metatable
	static int open(lua_State *state)
	{
		Type::name = lua_tostring(state, 1);
		luaL_newmetatable(state, Type::name);
		
		lua_pushliteral(state, "new");
		lua_pushcfunction(state, __new);
		lua_settable(state, -3);

		lua_pushliteral(state, "__tostring");
		lua_pushcfunction(state, __tostring);
		lua_settable(state, -3);

		lua_pushliteral(state, "__call");
		lua_pushcfunction(state, __call);
		lua_settable(state, -3);

		lua_pushliteral(state, "__gc");
		lua_pushcfunction(state, __gc);
		lua_settable(state, -3);

		lua_pushliteral(state, "__index");
		lua_newtable(state);
		luaL_Reg index[] =
		{
		{"min", min},
		{"max", max},
		{"uniform", uniform},
		{"bernoulli", bernoulli},
		{"binomial", binomial},
		{"geometric", geometric},
		{"negative", negative_binomial},
		{"poisson", poisson},
		{"exponential", exponential},
		{"gamma", gamma},
		{"weibull", weibull},
		{"extreme", extreme_value},
		{"normal", normal},
		{"lognormal", lognormal},
		{"chi_squared", chi_squared},
		{"cauchy", cauchy},
		{"fisher", fisher},
		{nullptr}
		};
		luaL_setfuncs(state, regs, 0);
		luaL_setfuncs(state, index, 0);
		lua_settable(state, -3);

		return 1;
	}

	// For sub-classes (see below)
	static luaL_Reg regs[];
};

// Specialized class for genuine random number device

static int entropy(lua_State *state)
{
	typedef lux_Store<std::random_device> Type;
	Type *user = Type::check(state);
	auto value = user->data.entropy();
	lux_push(state, value);
	return 1;
}

template <> luaL_Reg Random<std::random_device>::regs[] =
{
	{"entropy", entropy}, {nullptr}
};

// Specialized class for pseudo random number engine

template <class Base> static int seed(lua_State *state)
{
	typedef lux_Store<Base> Type;
	Type *user = Type::check(state, 1);
	int value = luaL_checkinteger(state, 2);
	user->data.seed(value);
	return 0;
}

template <class Base> static int discard(lua_State *state)
{
	typedef lux_Store<Base> Type;
	Type *user = Type::check(state, 1);
	int value = luaL_checkinteger(state, 2);
	user->data.discard(value);
	return 0;
}

template <class Base> luaL_Reg Random<Base>::regs[] =
{
	{"seed", seed<Base>}, {"discard", discard<Base>}, {nullptr}
};

// Lua module entry point

extern "C" int luaopen_random(lua_State *state)
{
	lua_newtable(state);
	luaL_Reg regs [] =
	{
	{"device", Random<std::random_device>::open},
	{"engine", Random<std::default_random_engine>::open},
	{"minstd_rand", Random<std::minstd_rand>::open},
	{"minstd_rand0", Random<std::minstd_rand0>::open},
	{"mt19937", Random<std::mt19937>::open},
	{"mt19937_64", Random<std::mt19937_64>::open},
	{"ranlux24_base", Random<std::ranlux24_base>::open},
	{"ranlux48_base", Random<std::ranlux48_base>::open},
	{"ranlux24", Random<std::ranlux24>::open},
	{"ranlux48", Random<std::ranlux48>::open},
	{"knuth_b", Random<std::knuth_b>::open},
	{nullptr}
	};
	for (auto reg=regs; reg->name; reg++)
	{
	 luaL_requiref(state, reg->name, reg->func, false);
	 lua_setfield(state, -2, reg->name);
	}
	return 1;
}

