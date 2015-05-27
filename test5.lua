print('test5: exporting the SDL2 interface to Lua')

window = SDL.CreateWindow('Main Window', 100, 100, 320, 240, SDL.WINDOW_SHOWN)
SDL.ShowSimpleMessageBox(0, 'Message Window', 'Click OK to exit.', window)
SDL.DestroyWindow(window)
