#include "lux.hpp" 
#include <cstdio>


struct Record
{
	union {
	char byte;
	short word;
	};
	double real;

	void say(void)
	{
		puts("Hello World!");
	}
};

template <> const char *lux_Union<Record>::Type::name = "Record";
template <> luaL_Reg lux_Union<Record>::newindex[] =
{
	lux_newindex(Record, word),
	lux_newindex(Record, real)
};
template <> luaL_Reg lux_Union<Record>::index[] =
{
	lux_index(Record, byte),
	lux_index(Record, real),
	lux_reg(Record, say)
};


int main(int argc, char **argv)
{
	lua_State *vm = luaL_newstate();
	luaL_openlibs(vm);

	lux_Union<Record>::open(vm);

	luaL_dofile(vm, "test3.lua");
	lux_stackdump(vm);
	lua_close(vm);
}


