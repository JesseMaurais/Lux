#include <iostream>
#include <SDL2/SDL.h>
#include "lux.hpp"


template <> const char *lux_Type<SDL_Window*>::name = "Window";
template <> const char *lux_Type<SDL_Surface*>::name = "Surface";


template <> void lux_push<SDL_bool>(lua_State *state, SDL_bool value)
{
	lua_pushboolean(state, value);
}
template <> SDL_bool lux_to<SDL_bool>(lua_State *state, int index)
{
	return (SDL_bool) lua_toboolean(state, index);
}


static int luxopen_SDL2(lua_State *state)
{
	// Register functions

	lua_newtable(state);
	luaL_Reg regs[] =
	{
	 {"CreateWindow", lux_cast(SDL_CreateWindow)},
	 {"DestroyWindow", lux_cast(SDL_DestroyWindow)},
	 {"DisableScreenSaver", lux_cast(SDL_DisableScreenSaver)},
	 {"EnableScreenSaver", lux_cast(SDL_EnableScreenSaver)},
	 //{"GetClosestDisplayMode", lux_cast(SDL_GetClosestDisplayMode)},
	 //{"GetCurrentDisplayMode", lux_cast(SDL_GetCurrentDisplayMode)},
	 {"GetCurrentVideoDriver", lux_cast(SDL_GetCurrentVideoDriver)},
	 //{"GetDesktopDisplayMode", lux_cast(SDL_GetDesktopDisplayMode)},
	 //{"GetDisplayBounds", lux_cast(SDL_GetDisplayBounds)},
	 //{"GetDisplayMode", lux_cast(SDL_GetDisplayMode)},
	 {"GetDisplayName", lux_cast(SDL_GetDisplayName)},
	 //{"GetGrabbedWindow", lux_cast(SDL_GetGrabbedWindow)},
	 {"GetNumDisplayModes", lux_cast(SDL_GetNumDisplayModes)},
	 {"GetNumVideoDisplays", lux_cast(SDL_GetNumVideoDisplays)},
	 {"GetNumVideoDrivers", lux_cast(SDL_GetNumVideoDrivers)},
	 {"GetVideoDriver", lux_cast(SDL_GetVideoDriver)},
	 {"GetWindowBrightness", lux_cast(SDL_GetWindowBrightness)},
	 {"GetWindowData", lux_cast(SDL_GetWindowData)},
	 {"GetWindowDisplayIndex", lux_cast(SDL_GetWindowDisplayIndex)},
	 //{"GetWindowDisplayMode", lux_cast(SDL_GetWindowDisplayMode)},
	 {"GetWindowFlags", lux_cast(SDL_GetWindowFlags)},
	 {"GetWindowFromID", lux_cast(SDL_GetWindowFromID)},
	 //{"GetWindowGammaRamp", lux_cast(SDL_GetWindowGammaRamp)},
	 {"GetWindowGrab", lux_cast(SDL_GetWindowGrab)},
	 {"GetWindowID", lux_cast(SDL_GetWindowID)},
	 //{"GetWindowMaximumSize", lux_cast(SDL_GetWindowMaximumSize)},
	 //{"GetWindowMinimumSize", lux_cast(SDL_GetWindowMinimumSize)},
	 {"GetWindowPixelFormat", lux_cast(SDL_GetWindowPixelFormat)},
	 //{"GetWindowPosition", lux_cast(SDL_GetWindowPosition)},
	 //{"GetWindowSize", lux_cast(SDL_GetWindowSize)},
	 {"GetWindowSurface", lux_cast(SDL_GetWindowSurface)},
	 {"GetWindowTitle", lux_cast(SDL_GetWindowTitle)},
	 //{"GetWindowWMInfo", lux_cast(SDL_GetWindowWMInfo)},
	 {"HideWindow", lux_cast(SDL_HideWindow)},
	 {"IsScreenSaverEnabled", lux_cast(SDL_IsScreenSaverEnabled)},
	 {"MaximizeWindow", lux_cast(SDL_MaximizeWindow)},
	 {"MinimizeWindow", lux_cast(SDL_MinimizeWindow)},
	 {"RaiseWindow", lux_cast(SDL_RaiseWindow)},
	 {"RestoreWindow", lux_cast(SDL_RestoreWindow)},
	 {"SetWindowBordered", lux_cast(SDL_SetWindowBordered)},
	 {"SetWindowBrightness", lux_cast(SDL_SetWindowBrightness)},
	 {"SetWindowData", lux_cast(SDL_SetWindowData)},
	 //{"SetWindowDisplayMode", lux_cast(SDL_SetWindowDisplayMode)},
	 {"SetWindowFullscreen", lux_cast(SDL_SetWindowFullscreen)},
	 //{"SetWindowGammaRamp", lux_cast(SDL_SetWindowGammaRamp)},
	 {"SetWindowGrab", lux_cast(SDL_SetWindowGrab)},
	 //{"SetWindowHitTest", lux_cast(SDL_SetWindowHitTest)},
	 {"SetWindowIcon", lux_cast(SDL_SetWindowIcon)},
	 {"SetWindowMaximumSize", lux_cast(SDL_SetWindowMaximumSize)},
	 {"SetWindowMinimumSize", lux_cast(SDL_SetWindowMinimumSize)},
	 {"SetWindowPosition", lux_cast(SDL_SetWindowPosition)},
	 {"SetWindowSize", lux_cast(SDL_SetWindowSize)},
	 {"SetWindowTitle", lux_cast(SDL_SetWindowTitle)},
	 //{"ShowMessageBox", lux_cast(SDL_ShowMessageBox)},
	 {"ShowSimpleMessageBox", lux_cast(SDL_ShowSimpleMessageBox)},
	 {"ShowWindow", lux_cast(SDL_ShowWindow)},
	 {"UpdateWindowSurface", lux_cast(SDL_UpdateWindowSurface)},
	 //{"UpdateWindowSurfaceRects", lux_cast(SDL_UpdateWindowSurfaceRects)},
	 {"VideoInit", lux_cast(SDL_VideoInit)},
	 {"VideoQuit", lux_cast(SDL_VideoQuit)},
	 {nullptr, nullptr}
	};
	luaL_setfuncs(state, regs, 0);

	// Register classes

	typedef lux_Type<SDL_Window*> Window;
	luaL_newmetatable(state, Window::name);
	lua_setfield(state, -2, Window::name);
	typedef lux_Type<SDL_Surface*> Surface;
	luaL_newmetatable(state, Surface::name);
	lua_setfield(state, -2, Surface::name);

	// Register constants

	lua_pushinteger(state, SDL_WINDOW_SHOWN);
	lua_setfield(state, -2, "WINDOW_SHOWN");
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

	lua_State *state = luaL_newstate();
	if (!state)
	{
	 std::cout << "Out of memory" << std::endl;
	 return 0;
	}

	luaL_openlibs(state);
	luxopen_array(state);
	luxopen_SDL2(state);
	luaL_dofile(state, "test5.lua");
	lux_stackdump(state);
	lua_close(state);
	return 0;
}

