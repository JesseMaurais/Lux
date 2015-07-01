#include "lux.hpp"
#include <random>


/*
 * Finds an optional function argument, buried in a table at index arg, and
 * returns it, unless arg is nil or the field name is not in the table, in
 * which case it turns n as a fallback value.
 */

static lua_Number opt(lua_State *state, int arg, const char *name, lua_Number n)
{
	// is index 'arg' a table?
	if (lua_istable(state, arg))
	{
		// does the table have a numeric field 'name' 
		if (lua_getfield(state, arg, name) == LUA_TNUMBER)
		{
			// copy that number here
			n = lua_tonumber(state, -1);
		}
		// remove the value
		lua_pop(state, 1);
	}
	return n;
}

/*
 * Base functions for random number generating classes, either genuine random
 * devices or pseudo random engines. 
 */

template <class C> struct Random
{
	// globally visible alias
	typedef lux_Type<C> Type;

	// return the box-pointer to a full userdata
	static Type *check(lua_State *state, int index)
	{
		union {
		 void *address;
		 Type *user;
		};
		address = luaL_checkudata(state, index, Type::name);
		return user;
	}

	// create an object of class C and set it's metatable
	static int __new(lua_State *state)
	{
		new (state) Type;
		luaL_setmetatable(state, Type::name);
		return 1;
	}

	// on garbage collection, call object's destructor
	static int __gc(lua_State *state)
	{
		auto user = check(state, 1);
		user->data.~C();
		return 0;
	}

	// generate a sequence of random numbers
	static int __call(lua_State *state)
	{
		auto user = check(state, 1);
		// if many, store them in a table
		size_t size = luaL_optinteger(state, 2, 0);
		if (size)
		{
			// user supplied a table?
			if (lua_gettop(state) < 3)
			{
				// put in a new table
				lua_newtable(state);
			}
			else
			{
				// put in user's table
				lua_pushvalue(state, 3);
			}
			// generate random numbers in the table
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
			// generate just one number
			auto value = user->data();
			lux_push(state, value);
		}
		return 1;
	}

	// similar to above but following a given probability distribution
	template <class D> static int generate(lua_State *state, D &distribution)
	{
		auto user = check(state, 1);
		// if many, store them in a table
		size_t size = luaL_optinteger(state, 2, 0);
		if (size)
		{
			// user supplied a table?
			if (lua_gettop(state) < 4)
			{
				// put in a new table
				lua_newtable(state);
			}
			else
			{
				// put in user's table
				lua_pushvalue(state, 4);
			}
			// generate random numbers in the table
			for (int item = 0; item < size; ++item)
			{
				lua_pushinteger(state, item);
				auto value = distribution(user->data);
				lux_push(state, value);
				lua_settable(state, -3);
			}
		}
		else
		{
			// generate just one number
			auto value = distribution(user->data);
			lux_push(state, value);
		}
		return 1;
	}

	// continuous uniform random numbers
	static int uniform(lua_State *state)
	{
		// distribution parameters
		auto min = opt(state, 3, "min", 0);
		auto max = opt(state, 3, "max", 1);
		// probability distribution object
		std::uniform_real_distribution<lua_Number> distribution(min, max);
		// instantiate the generator template function
		return generate(state, distribution);
	}

	// (continuous) exponential random numbers
	static int exponential(lua_State *state)
	{
		// distribution parameters
		auto rate = luaL_optnumber(state, 3, 1.0);
		// probability distribution object
		std::exponential_distribution<lua_Number> distribution(rate);
		// instantial the generate template function
		return generate(state, distribution);
	}

	// device/engine's minimum value
	static int min(lua_State *state)
	{
		auto user = check(state, 1);
		auto value = user->data.min();
		lux_push(state, value);
		return 1;
	}

	// device/engine's maximum value
	static int max(lua_State *state)
	{
		auto user = check(state, 1);
		auto value = user->data.max();
		lux_push(state, value);
		return 1;
	}

	// create and fill a metatable
	static int open(lua_State *state)
	{
		Type::name = lua_tostring(state, -1);
		luaL_newmetatable(state, Type::name);
		
		lua_pushliteral(state, "new");
		lua_pushcfunction(state, __new);
		lua_settable(state, -3);

		lua_pushliteral(state, "__call");
		lua_pushcfunction(state, __call);
		lua_settable(state, -3);

		lua_pushliteral(state, "__gc");
		lua_pushcfunction(state, __gc);
		lua_settable(state, -3);

		lua_pushliteral(state, "__index");
		lua_newtable(state);
		luaL_Reg common [] =
		{
		 {"min", min},
		 {"max", max},
		 {"uniform", uniform},
		 {"exponential", exponential},
		 {nullptr, nullptr}
		};
		luaL_setfuncs(state, regs, 0);
		luaL_setfuncs(state, common, 0);
		lua_settable(state, -3);

		return 1;
	}

	static void require(lua_State *state, const char *name, bool global=false)
	{
		luaL_requiref(state, name, open, global);
	}

	// for sub-classes (see below)
	static luaL_Reg regs[];
};



/*
 * Specialized class for genuine random number device
 */

static int entropy(lua_State *state)
{
	typedef Random<std::random_device> Random;
	auto user = Random::check(state, 1);
	auto value = user->data.entropy();
	lux_push(state, value);
	return 1;
}

template <> luaL_Reg Random<std::random_device>::regs[] =
{
 {"entropy", entropy}, {nullptr, nullptr}
};


/*
 * Specialized class for pseudo random number engine
 */


template <class C> static int seed(lua_State *state)
{
	auto user = Random<C>::check(state, 1);
	int value = luaL_checkinteger(state, 2);
	user->data.seed(value);
	return 0;
}

template <class C> static int discard(lua_State *state)
{
	auto user = Random<C>::check(state, 1);
	int number = luaL_checkinteger(state, 2);
	user->data.discard(number);
	return 0;
}

template <class C> luaL_Reg Random<C>::regs[] =
{
 {"seed", seed<C>}, {"discard", discard<C>}, {nullptr, nullptr}
};



/*
 * Lua module entry point
 */

extern "C" int luaopen_random(lua_State *state)
{
	lua_newtable(state);
	Random<std::random_device>::require(state, "device");
	Random<std::default_random_engine>::require(state, "default_engine");
	return 1;
}

