/**
 * Support for numeric limits; reporting the representation, precision, style
 * of rounding error, etc, for common numeric types. The methods provided are
 * stored in the same metatables as those created by the array module so that
 * there is only one place to for all the functions concerning numeric types.
 * This also makes it easy to inherit all such methods in Lua.
 */

#include "lux.hpp"
#include <limits>

template <> inline
int lux_push(lua_State *state, std::float_round_style style)
{
	switch (style)
	{
	case std::round_indeterminate:
		lua_pushliteral(state, "indeterminate");
		break;
	case std::round_toward_zero:
		lua_pushliteral(state, "toward zero");
		break;
	case std::round_to_nearest:
		lua_pushliteral(state, "to nearest");
		break;
	case std::round_toward_infinity:
		lua_pushliteral(state, "toward infinity");
		break;
	case std::round_toward_neg_infinity:
		lua_pushliteral(state, "toward -infinity");
		break;
	default:
		return 0;
	}
	return 1;
}

template <> inline
int lux_push(lua_State *state, std::float_denorm_style style)
{
	switch (style)
	{
	case std::denorm_indeterminate:
		lua_pushliteral(state, "indeterminate");
		break;
	case std::denorm_absent:
		lua_pushliteral(state, "absent");
		break;
	case std::denorm_present:
		lua_pushliteral(state, "present");
		break;
	default:
		return 0;
	}
	return 1;
}

template <class C> struct lux_Limits
{
	using limits = std::numeric_limits<C>;

	static int min(lua_State *state)
	{
		return lux_push(state, limits::min());
	}

	static int max(lua_State *state)
	{
		return lux_push(state, limits::max());
	}

	static int lowest(lua_State *state)
	{
		return lux_push(state, limits::lowest());
	}

	static int digits(lua_State *state)
	{
		return lux_push(state, limits::digits);
	}

	static int digits10(lua_State *state)
	{
		return lux_push(state, limits::digits10);
	}

	static int max_digits10(lua_State *state)
	{
		return lux_push(state, limits::max_digits10);
	}

	static int is_exact(lua_State *state)
	{
		return lux_push(state, limits::is_exact);
	}

	static int is_signed(lua_State *state)
	{
		return lux_push(state, limits::is_signed);
	}

	static int is_integer(lua_State *state)
	{
		return lux_push(state, limits::is_integer);
	}
	
	static int radix(lua_State *state)
	{
		return lux_push(state, limits::radix);
	}

	static int epsilon(lua_State *state)
	{
		return lux_push(state, limits::epsilon());
	}

	static int round_error(lua_State *state)
	{
		return lux_push(state, limits::round_error());
	}

	static int min_exponent(lua_State *state)
	{
		return lux_push(state, limits::min_exponent);
	}

	static int min_exponent10(lua_State *state)
	{
		return lux_push(state, limits::min_exponent10);
	}

	static int max_exponent(lua_State *state)
	{
		return lux_push(state, limits::max_exponent);
	}

	static int max_exponent10(lua_State *state)
	{
		return lux_push(state, limits::max_exponent10);
	}

	static int has_infinity(lua_State *state)
	{
		return lux_push(state, limits::has_infinity);
	}

	static int has_quiet_NaN(lua_State *state)
	{
		return lux_push(state, limits::has_quiet_NaN);
	}
	
	static int has_signaling_NaN(lua_State *state)
	{
		return lux_push(state, limits::has_signaling_NaN);
	}

	static int has_denorm(lua_State *state)
	{
		return lux_push(state, limits::has_denorm);
	}
	
	static int has_denorm_loss(lua_State *state)
	{
		return lux_push(state, limits::has_denorm_loss);
	}

	static int infinity(lua_State *state)
	{
		return lux_push(state, limits::infinity());
	}

	static int quiet_NaN(lua_State *state)
	{
		return lux_push(state, limits::quiet_NaN());
	}

	static int signaling_NaN(lua_State *state)
	{
		return lux_push(state, limits::signaling_NaN());
	}

	static int denorm_min(lua_State *state)
	{
		return lux_push(state, limits::denorm_min());
	}

	static int is_iec559(lua_State *state)
	{
		return lux_push(state, limits::is_iec559);
	}

	static int is_bounded(lua_State *state)
	{
		return lux_push(state, limits::is_bounded);
	}

	static int is_modulo(lua_State *state)
	{
		return lux_push(state, limits::is_modulo);
	}

	static int traps(lua_State *state)
	{
		return lux_push(state, limits::traps);
	}

	static int tinyness_before(lua_State *state)
	{
		return lux_push(state, limits::tinyness_before);
	}

	static int round_style(lua_State *state)
	{
		return lux_push(state, limits::round_style);
	}
	

	static int open(lua_State *state)
	{
		// Pull metatable name of the stack
		auto name =  lua_tostring(state, -1);
		// Acquire the metatable
		luaL_newmetatable(state, name);
		// Store methods in it
		luaL_Reg regs[] =
		{
		{"min", min},
		{"max", max},
		{"lowest", lowest},
		{"digits", digits},
		{"digits10", digits10},
		{"max_digits10", max_digits10},
		{"is_exact", is_exact},
		{"is_signed", is_signed},
		{"is_integer", is_integer},
		{"radix", radix},
		{"epsilon", epsilon},
		{"round_error", round_error},
		{"min_exponent", min_exponent},
		{"min_exponent10", min_exponent10},
		{"max_exponent", max_exponent},
		{"max_exponent10", max_exponent10},
		{"has_infinity", has_infinity},
		{"has_quiet_NaN", has_quiet_NaN},
		{"has_signaling_NaN", has_signaling_NaN},
		{"has_denorm", has_denorm},
		{"has_denorm_loss", has_denorm_loss},
		{"infinity", infinity},
		{"quiet_NaN", quiet_NaN},
		{"signaling_NaN", signaling_NaN},
		{"denorm_min", denorm_min},
		{"is_iec559", is_iec559},
		{"is_bounded", is_bounded},
		{"is_modulo", is_modulo},
		{"traps", traps},
		{"tinyness_before", tinyness_before},
		{"round_style", round_style},
		{nullptr}
		};
		luaL_setfuncs(state, regs, 0);
		return 1;
	}
};


extern "C" int luaopen_limits(lua_State *state)
{
	lua_newtable(state);
	int tab = lua_gettop(state);
	// Common numeric types
	luaL_Reg regs[] =
	{
	{lux_Array<int>::Store::name, lux_Limits<int>::open},
	{lux_Array<char>::Store::name, lux_Limits<char>::open},
	{lux_Array<short>::Store::name, lux_Limits<short>::open},
	{lux_Array<long>::Store::name, lux_Limits<long>::open},
	{lux_Array<float>::Store::name, lux_Limits<float>::open},
	{lux_Array<double>::Store::name, lux_Limits<double>::open},
	{nullptr}
	};
	// Register and put in module table
	for (auto reg=regs; reg->name; ++reg)
	{
		// Put the metatable in the stack
		lua_pushstring(state, reg->name);
		reg->func(state); // new interface
		// Store in returned module table
		lua_setfield(state, tab, reg->name);
		// Pop metatable name
		lua_pop(state, 1);
	}
	// Done
	return 1;
}

