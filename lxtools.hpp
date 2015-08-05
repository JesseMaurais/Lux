#ifndef __lxtools__
#define __lxtools__

/*
 * This is where I'm putting new ideas that don't seem to fit anywhere else.
 */

#include "lxstack.hpp"
#include <cstring>

// Similar to luaL_Reg but for all classes for which lux_push is defined

template <class User> struct lux_Reg
{
	const char *name;
	User value;
};

template <class User>
void lux_settable(lua_State *state, const lux_Reg<User> regs[])
{
	for (auto r=regs; r->name; ++r)
	{
		lua_pushstring(state, r->name);
		lux_push<User>(state, r->value);
		lua_settable(state, -3);
	}
}

// Optional argument like luaL_checkoption for converting between string/enums

template <class User> struct lux_Opt
{
	typedef lux_Reg<User> Reg;

	const static Reg args[];

	static void push(lua_State *state, User value, const char *opt)
	{
		lua_pushstring(state, find(value, args, opt));
	}

	static User opt(lua_State *state, int stack, User opt)
	{
		return find(lua_tostring(state, stack), args, opt);
	}

 private:

	template <size_t Size> static
	 User find(const char *name, const Reg regs[Size], User opt)
	{
		lux_Reg<User> key = {name};
		union {
		 lux_Reg<User> *reg;
		 void *address;
		};
		reg = &key;
		size_t size = Size;
		address = lfind(address, regs, &size, sizeof(key), compare_n);
		return reg ? reg->value : opt;
	}

	template <size_t Size> static
	const char *find(User value, const Reg regs[Size], const char *opt)
	{
		lux_Reg<User> key = {nullptr, value};
		union {
		 lux_Reg<User> *reg;
		 void *address;
		};
		reg = &key;
		size_t size = Size;
		address = lfind(address, regs, &size, sizeof(key), compare_v);
		return reg ? reg->name : opt;
	}

 private:

	static int compare_n(const void *p1, const void *p2)
	{
		const Reg *r1 = (const Reg *) p1;
		const Reg *r2 = (const Reg *) p2;
		return strcmp(r1->name, r2->name);
	}

	static int compare_v(const void *p1, const void *p2)
	{
		const Reg *r1 = (const Reg *) p1;
		const Reg *r2 = (const Reg *) p2;
		return r1->value == r2->value;
	}
};

#endif // file

