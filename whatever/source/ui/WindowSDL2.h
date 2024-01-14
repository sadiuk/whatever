#pragma once
#include "IWindow.h"
#include "SDL.h"
#include <string>

class WindowSDL2 : public IWindow
{
public:
	WindowSDL2(const CreationParams& params);
	void* GetNativeHandle() override;
	bool IsOpen() override;
	WINDOW_LIB GetWindowingLib() { return SDL2; }
private:

	SDL_Window* m_window;
};