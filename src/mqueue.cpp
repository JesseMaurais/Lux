/**
 * Support for inter-process message queues using the POSIX interface. We only
 * pass string data as messages here. 
 */

#include "lux.hpp"
#include <mqueue.h>

static int l_open(lua_State *state)
{
	const mqd_t invalid = (mqd_t) -1;
	mqd_t mq = mq_open(lua_tostring(state, 1), lua_tointeger(state, 2));
	if (mq == invalid) return lux_perror(state);
	lux_push(state, mq);
	return 1;
}

static int l_close(lua_State *state)
{
	mqd_t mq = lux_to<mqd_t>(state, 1);
	if (mq_close(mq) < 0) return lux_perror(state);
	return 0;
}

static int l_send(lua_State *state)
{
	mqd_t mq = lux_to<mqd_t>(state, 1);
	size_t len;
	const char *msg = lua_tolstring(state, 2, &len);
	unsigned prio = luaL_optinteger(state, 3, 0);
	if (mq_send(mq, msg, len, prio) < 0) return lux_perror(state);
	return 0;
}

static int l_receive(lua_State *state)
{
	mqd_t mq = lux_to<mqd_t>(state, 1);
	size_t len = luaL_optinteger(state, 2, FILENAME_MAX);
	char msg[len];
	unsigned prio;
	if (mq_receive(mq, msg, len, &prio) < 0) return lux_perror(state);
	lua_pushlstring(state, msg, len);
	lua_pushinteger(state, prio);
	return 2;
}

static int l_unlink(lua_State *state)
{
	if (mq_unlink(lua_tostring(state, 1)) < 0) return lux_perror(state);
	return 0;
}

template <> luaL_Reg lux_Class<mq_attr>::index[] =
	{
	{"mq_flags", lux_member(mq_attr, mq_flags)},
	{"mq_maxmsg", lux_member(mq_attr, mq_maxmsg)},
	{"mq_msgsize", lux_member(mq_attr, mq_msgsize)},
	{"mq_curmsgs", lux_member(mq_attr, mq_curmsgs)},
	nullptr
	};

static int l_getattr(lua_State *state)
{
	mqd_t mq = lux_to<mqd_t>(state, 1);
	mq_attr *attr = lux_to<mq_attr*>(state, 2);
	if (mq_getattr(mq, attr) < 0) return lux_perror(state);
	return 0;
}

static int l_setattr(lua_State *state)
{
	mqd_t mq = lux_to<mqd_t>(state, 1);
	mq_attr *attr = lux_to<mq_attr*>(state, 2);
	mq_attr *old = lux_opt<mq_attr*>(state, 3, nullptr);
	if (mq_setattr(mq, attr, old) < 0) return lux_perror(state);
	return 0;
}

extern "C" int luaopen_mqueue(lua_State *state)
{
	if (lux_Class<mq_attr>::open(state))
	{
		luaL_Reg regs[] =
		{
		{"open", l_open},
		{"close", l_close},
		{"send", l_send},
		{"receive", l_receive},
		{"unlink", l_unlink},
		{"setattr", l_setattr},
		{"getattr", l_getattr},
		nullptr
		};
		luaL_setfuncs(state, regs, 0);
		return 1;
	}
	return luaL_error(state, "Cannot create module table");
}

