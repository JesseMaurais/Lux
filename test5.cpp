#include <iostream>
#include <SDL2/SDL.h>
#include "lux.hpp"


template <> const char *lux_Type<SDL_Window*>::name = "Window";

template <> void lux_push<SDL_bool>(lua_State *state, SDL_bool value)
{
	lua_pushboolean(state, value);
}

template <> SDL_bool lux_to<SDL_bool>(lua_State *state, int index)
{
	return (SDL_bool) lua_toboolean(state, index);
}


static int lux_SDL2(lua_State *state)
{
	lua_newtable(state);

	// Register types

	lux_newtype<SDL_Window*>(state);

	// Register functions

	luaL_Reg regs[] =
	{
	 {"GetError", lux_cast(SDL_GetError)},
	 {"CreateWindow", lux_cast(SDL_CreateWindow)},
	 {"DestroyWindow", lux_cast(SDL_DestroyWindow)},
	 {"DisableScreenSaver", lux_cast(SDL_DisableScreenSaver)},
	 {"EnableScreenSaver", lux_cast(SDL_EnableScreenSaver)},
	 {"GetCurrentVideoDriver", lux_cast(SDL_GetCurrentVideoDriver)},
	 {"GetDisplayName", lux_cast(SDL_GetDisplayName)},
	 {"GetNumVideoDrivers", lux_cast(SDL_GetNumVideoDrivers)},
	 {"GetVideoDriver", lux_cast(SDL_GetVideoDriver)},
	 {"GetWindowDisplayIndex", lux_cast(SDL_GetWindowDisplayIndex)},
	 {"GetWindowFromID", lux_cast(SDL_GetWindowFromID)},
	 {"GetWindowID", lux_cast(SDL_GetWindowID)},
	 {"HideWindow", lux_cast(SDL_HideWindow)},
	 {"ShowSimpleMessageBox", lux_cast(SDL_ShowSimpleMessageBox)},
	 {"ShowWindow", lux_cast(SDL_ShowWindow)},
	 {nullptr, nullptr}
	};
	luaL_setfuncs(state, regs, 0);
	lua_setglobal(state, "SDL");
	return 0;
}

int main(int argc, char **argv)
{
	if (SDL_Init(SDL_INIT_EVERYTHING))
	{
	 std::cout << SDL_GetError() << std::endl;
	 return 0;
	}
	else
	 atexit(SDL_Quit);

	auto state = luaL_newstate();
	luaL_openlibs(state);
	lux_openarray(state);
	lux_SDL2(state);
	luaL_dofile(state, "test5.lua");
	lux_stackdump(state);
	lua_close(state);
	return 0;
}

