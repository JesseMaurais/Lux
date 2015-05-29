print('test5: exporting the SDL2 interface to Lua')

SDL.DisableScreenSaver()

print('Current video driver is ' .. SDL.GetCurrentVideoDriver() )

window = SDL.CreateWindow('Main Window', 100, 100, 320, 240, 0)

index = SDL.GetWindowDisplayIndex(window)
print('Window is on display ' .. SDL.GetDisplayName(index) )

SDL.ShowWindow(window)
SDL.ShowSimpleMessageBox(0, 'Message Window', 'Click OK to exit.', window)

SDL.HideWindow(window)
SDL.DestroyWindow(window)
SDL.EnableScreenSaver()
