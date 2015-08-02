#ifndef __lxerror__
#define __lxerror__

#include <cstring>
#include <cerrno>

/// Imitate the behavior of the perror function in a given state
static inline int lux_perror(lua_State *state)
{
	return luaL_error(state, "%s", strerror(errno));
}

/// Report an errno string as coming from a function argument
static inline int lux_argerror(lua_State *state, int arg=1)
{
	return luaL_argerror(state, arg, strerror(errno));
}

/// Report an errno string if a certain condition goes unsatisfied
static inline void lux_argcheck(lua_State *state, bool cond, int arg=1)
{
	luaL_argcheck(state, cond, arg, strerror(errno));
}

/// When you want to report the failed condition as the string message
#define lux_checkarg(state, arg, cond) luaL_argcheck(state, (cond), arg, #cond)

/* Many C library functions set errno and return a flag such as a negative
 * number when they fail to complete successfully. For example, in UNIX:
 *
 *	if (pipe(fd) < 0) perror("pipe");
 *
 * Can be translated in a lua_CFunction as
 *
 *	if (pipe(fd) < 0) return lux_perror(state);
 *	else return lux_push(state, fd[0], fd[1]);
 *
 * And the fopen function might translated
 *
 *	auto path = lua_tostring(state, arg);
 *	auto file = fopen(path, "rw");
 *	if (!file) return lux_argerror(state, arg);
 *	else lux_push(state, file);
 */

#endif // file

