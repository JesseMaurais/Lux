#ifndef __lxerror__
#define __lxerror__

#include <cstring>
#include <cerrno>

/// Imitate the behavior of the perror function in a given state
static inline int lux_perror(lua_State *state, const char *s)
{
	return luaL_error(state, "%s: %s", s, strerror(errno));
}

/// Report an errno string as coming from a function argument
static inline int lux_argerror(lua_State *state, int arg=1)
{
	return luaL_error(state, arg, strerror(errno));
}

/// Report an errno string if a certain condition goes unsatisfied
static inline void lux_argcheck(lua_State *state, bool cond, int arg=1)
{
	luaL_argcheck(state, cond, arg, strerror(errno));
}

/* Many C library functions set errno and return a flag such as a negative
 * number when the fail to complete successfully. For example, in UNIX:
 *
 *	if (pipe(fd) < 0) perror("pipe");
 *
 * Can be translated in a lua_CFunction as
 *
 *	if (pipe(fd) < 0) return lux_perror(state, "pipe");
	else return lux_push(state, fd[0], fd[1]);
 */

#endif // file

