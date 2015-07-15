#include "lux.hpp"
#include <random>

/*
 * Finds an optional function argument, buried in a table at index arg, and
 * returns it, unless arg is nil or the field name is not in the table, in
 * which case it returns n as a fallback value.
 */

static lua_Number opt(lua_State *state, int arg, const char *name, lua_Number n)
{
	// is index 'arg' a table?
	if (lua_istable(state, arg))
	{
		// does the table have a numeric field 'name'?
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

template <class Base> struct Random
{
	// globally visible alias
	typedef lux_Type<Base> Type;

	// create an object of class Base and set its metatable
	static int __new(lua_State *state)
	{
		new (state) Type;
		luaL_setmetatable(state, Type::name);
		return 1;
	}

	// on garbage collection, call object's destructor
	static int __gc(lua_State *state)
	{
		Type *user = Type::check(state);
		user->data.~Base();
		return 0;
	}

	// for printing the name of this object
	static int __tostring(lua_State *state)
	{
		lua_pushfstring(state, "random.%s", Type::name);
		return 1;
	}

	// generate a sequence of random numbers
	static int __call(lua_State *state)
	{
		auto user = Type::check(state, 1);
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
	template <class Distribution>
	static int generate(lua_State *state, Distribution &distribution)
	{
		Type *user = Type::check(state, 1);
		// if many, store them in a table
		size_t size = opt(state, 2, "size", 0);
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
		lua_Number a, b;
		// provided parameter table?
		if (lua_istable(state, 3))
		{
		 a = opt(state, 3, "min", 0.0);
		 b = opt(state, 3, "max", 1.0);
		}
		// probability distribution object
		std::uniform_real_distribution<lua_Number> distribution(a, b);
		// instantiate the generator template function
		return generate(state, distribution);
	}

	// device/engine's minimum value
	static int min(lua_State *state)
	{
		Type *user = Type::check(state);
		auto value = user->data.min();
		lux_push(state, value);
		return 1;
	}

	// device/engine's maximum value
	static int max(lua_State *state)
	{
		Type *user = Type::check(state);
		auto value = user->data.max();
		lux_push(state, value);
		return 1;
	}

	// create and fill a metatable
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
		luaL_Reg common [] =
		{
		{"min", min},
		{"max", max},
		{"uniform", uniform},
		{nullptr, nullptr}
		};
		luaL_setfuncs(state, regs, 0);
		luaL_setfuncs(state, common, 0);
		lua_settable(state, -3);
		return 1;
	}

	// for sub-classes (see below)
	static luaL_Reg regs[];
};



/*
 * Specialized class for genuine random number device
 */

static int entropy(lua_State *state)
{
	typedef lux_Type<std::random_device> Type;
	Type *user = Type::check(state);
	auto value = user->data.entropy();
	lux_push(state, value);
	return 1;
}

template <> luaL_Reg Random<std::random_device>::regs[] =
{
 {"entropy", entropy}, {nullptr}
};


/*
 * Specialized class for pseudo random number engine
 */


template <class Base> static int seed(lua_State *state)
{
	typedef lux_Type<Base> Type;
	Type *user = Type::check(state, 1);
	int value = luaL_checkinteger(state, 2);
	user->data.seed(value);
	return 0;
}

template <class Base> static int discard(lua_State *state)
{
	typedef lux_Type<Base> Type;
	Type *user = Type::check(state, 1);
	int value = luaL_checkinteger(state, 2);
	user->data.discard(value);
	return 0;
}

template <class Base> luaL_Reg Random<Base>::regs[] =
{
 {"seed", seed<Base>}, {"discard", discard<Base>}, {nullptr}
};



/*
 * Lua module entry point
 */

extern "C" int luaopen_random(lua_State *state)
{
	lua_newtable(state);
	luaL_Reg regs [] =
	{
	{"device", Random<std::random_device>::open},
	{"engine", Random<std::default_random_engine>::open},
	{nullptr}
	};
	for (auto reg=regs; reg->name; reg++)
	{
	 luaL_requiref(state, reg->name, reg->func, false);
	 lua_setfield(state, -2, reg->name);
	}
	return 1;
}

