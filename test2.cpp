#include <cstdlib>
#include "lux.hpp" 


int main(int argc, char **argv)
{
	lua_State *state = luaL_newstate();
	luaL_openlibs(state);
	luxopen_array(state);

	luaL_dofile(state, "test2.lua");
	lux_stackdump(state);
	lua_close(state);
}


