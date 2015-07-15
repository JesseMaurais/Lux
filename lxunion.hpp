#ifndef __lxunion__
#define __lxunion__

#include "lxalloc.hpp"
#include "lxtypes.hpp"
#include "lxstack.hpp"


// sort a function registry by name
void lux_qsort(luaL_Reg *regs, size_t size);

// search a sorted function registry
lua_CFunction lux_bsearch(const char *key, const luaL_Reg *regs, size_t size);

// the number of items in an array of luaL_Reg
#define lux_regsize(regs) (sizeof(regs)/sizeof(luaL_Reg))
// this works assuming that regs is *not* dynamically allocated
#define lux_search(key, regs) lux_bsearch(key, regs, lux_regsize(regs))
#define lux_sort(regs) lux_qsort(regs, lux_regsize(regs))


// class prototype for accessing C structs & unions in Lua
template <class User> struct lux_Union
{
	typedef lux_Type<User*> Type;

	static luaL_Reg index[];
	static luaL_Reg newindex[];

	static int __new(lua_State *state)
	{
		size_t size = 1;
		User *data = nullptr;

		switch (lua_type(state, 1))
		{
		  case LUA_TNIL:
			// nullptr
			size = 0;
			break;
		  case LUA_TNUMBER:
			// array
			size = lux_to<int>(state, 1);
		  case LUA_TNONE:
			// single
			if (size) data = new User [size];
			break;
		  case LUA_TUSERDATA:
			// cast
			data = (User*) lua_touserdata(state, 1);
			size = 0;
			break;
		  case LUA_TTABLE:
			// constructor
			Type::push(state, new User, size);
			lua_pushnil(state);
			while (lua_next(state, 1))
			{
			 __newindex(state);
			 lua_pop(state, 1);
			}
			return 1;
		  default:
			return luaL_argerror(state, 1, "invalid type");
		};

		Type::push(state, data, size);
		return 1;
	}

	static int __gc(lua_State *state)
	{
		auto user = Type::check(state, 1);
		if (user->size) delete [] user->data;
		return 0;
	}

	static int __tostring(lua_State *state)
	{
		lua_pushfstring(state, "%s: %p", Type::name, Type::to(state));
		return 1;
	}

	static int __newindex(lua_State *state)
	{
		const char *key = luaL_checkstring(state, -2);
		if (newindex)
		{
		 auto set = lux_search(key, newindex);
		 if (set) return set(state);
		}
		return 0;
	}

	static int __index(lua_State *state)
	{
		const char *key = luaL_checkstring(state, -1);
		if (index)
		{
		 auto get = lux_search(key, index);
		 if (get) return get(state);
		}
		return 0;
	}

	static int __call(lua_State *state)
	{
		if (lua_istable(state, -1))
		{
			lua_pushnil(state);
			while (lua_next(state, -1))
			{
				__newindex(state);
				lua_pop(state, 1);
			}
		}
		return 0;
	}

	static int __len(lua_State *state)
	{
		auto user = Type::check(state, 1);
		lua_pushinteger(state, user->size);
		return 1;
	}

	static int __add(lua_State *state)
	{
		User *data = lux_to<User*>(state, 1);
		int offset = lux_to<int>(state, 2);
		lux_push(state, data + offset);
		return 1;
	}

	static int __sub(lua_State *state)
	{
		User *data = lux_to<User*>(state, 1);
		int offset = lux_to<int>(state, 2);
		lux_push(state, data - offset);
		return 1;
	}

	static void setfuncs(lua_State *state, int up=0)
	{
		luaL_Reg regs [] =
		{
		{"__tostring", __tostring},
		{"__newindex", __newindex},
		{"__index", __index},
		{"__call", __call},
		{"__len", __len},
		{"__add", __add},
		{"__sub", __sub},
		{"__gc", __gc},
		{"new", __new},
		{nullptr, nullptr}
		};
		luaL_setfuncs(state, regs, up);
	}

	static int open(lua_State *state)
	{
		// ensure these lists are sorted
		lux_sort(index);
		lux_sort(newindex);
		// register/get the metatable
		luaL_newmetatable(state, Type::name);
		setfuncs(state);
		// table[Type::name] = metatable
		lua_setfield(state, -2, Type::name);
		return 0;
	}

	/* For structs, classes, and unions this template logic applies
	 * struct C { X x; }; 
	 * Then offsetof(C, x) == offset
	 */

	// address of a member in a struct/union at stack index with given type
	template <class X, int offset> static X *at(lua_State *state, int index)
	{
		union {
		 User *buffer;
		 char *address;
		 X *object;
		};
		buffer = lux_to<User*>(state, index);
		address += offset;
		return object;
	}

	// function prototype for setting the value of a struct member
	template <class X, int offset> static int set(lua_State *state)
	{
		*at<X, offset>(state, -3) = lux_to<X>(state, -1);
		return 0;
	}

	// function prototype for getting the value of a struct member
	template <class X, int offset> static int get(lua_State *state)
	{
		lux_push<X>(state, *at<X, offset>(state, -2));
		return 1;
	}

	// function prototype for getting the address of sub-struct
	template <class X, int offset> static int sub(lua_State *state)
	{
		lux_push(state, at<X, offset>(state, -2));
	}
};

// for offsetof the member must be a standard layout type
#define lux_get(C, x) lux_Union<C>::get<decltype(C::x), offsetof(C, x)>
#define lux_set(C, x) lux_Union<C>::set<decltype(C::x), offsetof(C, x)>
#define lux_sub(C, x) lux_Union<C>::sub<decltype(C::x), offsetof(C, x)>

// these are valid luaL_Reg constructors
#define lux_index(C, x)    {#x, lux_get(C, x)}
#define lux_newindex(C, x) {#x, lux_set(C, x)}
#define lux_subindex(C, x) {#x, lux_sub(C, x)}

// default to empty unless specified otherwise
template <class User> luaL_Reg lux_Union<User>::index[] = {};
template <class User> luaL_Reg lux_Union<User>::newindex[] = {};

#endif // file

