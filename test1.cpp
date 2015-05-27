#include "lux.hpp"
#include <stdexcept>
#include <iostream>

void Die(std::string why)
{
	throw std::runtime_error(why);
}

void HelloWorld()
{
	std::cout << "Hello World!" << std::endl;
}

int Add(int a, int b)
{
	return a + b;
}

void Repeat(int a, int b, int c)
{
	std::cout << a << ", " << b << ", " << c << std::endl;
}

int main(int argc, char **argv)
{
	lua_State *state = luaL_newstate();
	luaL_openlibs(state);
	
	lux_register(state, HelloWorld);
	lux_register(state, Repeat);
	lux_register(state, Add);
	lux_register(state, Die);

	luaL_dofile(state, "test1.lua");
	lux_stackdump(state);
	lua_close(state);
}


