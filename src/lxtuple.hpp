#ifndef __lxtuple__
#define __lxtuple__

#include "lxstack.hpp"
#include <utility>
#include <tuple>

template <typename First, typename Second> inline
int lux_push(lua_State *state, const std::pair<First, Second> &pair)
{
	return lux_push(state, pair.first) + lux_push(state, pair.second);
}

template <typename... Types, size_t... Items> inline
int lux_push(lua_State *state, const std::tuple<Types...> &tuple, std::index_sequence<Items...>)
{
	return lux_push(state, std::get<Items>(std::forward(tuple))...);
}

template <typename... Types> inline
int lux_push(lua_State *state, const std::tuple<Types...> &tuple)
{
	constexpr auto Size = std::tuple_size<decltype(tuple)>::value;
	return lux_push(state, tuple, std::make_index_sequence<Size>{});
}

#endif // file

