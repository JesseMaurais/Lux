#ifndef __lxerror__
#define __lxerror__

/**
 * Many C library functions set errno and return a flag such as a negative
 * number when they fail to complete successfully. For example, on UNIX
 *
 *	if (pipe(fd) < 0) perror("pipe");
 *
 * Can be translated in a lua_CFunction as
 *
 *	if (pipe(fd) < 0) return lux_perror(state);
 *	else return lux_push(state, fd[0], fd[1]);
 *
 * And the fopen function might be translated
 *
 *	auto path = lua_tostring(state, arg);
 *	auto file = fopen(path, "rw");
 *	if (!file) return lux_argerror(state, arg);
 *	else lux_push(state, file);
 *
 * Also in Lux we often find something like
 *
 *	Type *array = Type::check(state, arg)
 *	size_t size = abs(array->size);
 *	lux_argcheck(state, arg, 0 < size);
 */

#include "lua.hpp"
#include <cstring>
#include <cerrno>

/// Imitate the behavior of perror in a given state
static inline int lux_perror(lua_State *state, int error = errno)
{
	return luaL_error(state, "%s", std::strerror(error));
}

/// Report an errno string as coming from a function argument
static inline int lux_argerror(lua_State *state, int arg=1)
{
	return luaL_argerror(state, arg, std::strerror(errno));
}

/// When you want to report the failed condition as the string message
#define lux_argcheck(state, arg, cond) luaL_argcheck(state, (cond), arg, #cond)

#endif // file

