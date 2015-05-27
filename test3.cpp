#include <sys/stat.h>
#include "lux.hpp" 


struct Record
{
	union {
	char byte;
	short word;
	};
	double real;
};

template <> const char *lux_Union<Record>::Type::name = "Record";

template <> int lux_Union<Record>::setfuncs(lua_State *state)
{
	set<decltype(Record::word), offsetof(Record, word)>("word");
	get<decltype(Record::byte), offsetof(Record, byte)>("byte");
	set<decltype(Record::real), offsetof(Record, real)>("real");
	get<decltype(Record::real), offsetof(Record, real)>("real");
}

int main(int argc, char **argv)
{
	lua_State *vm = luaL_newstate();
	luaL_openlibs(vm);

	lux_Union<Record>::open(vm);
	luaL_dofile(vm, "test3.lua");
	lux_stackdump(vm);
	lua_close(vm);
}


