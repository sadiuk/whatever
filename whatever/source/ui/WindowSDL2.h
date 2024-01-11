#pragma once
#include "IWindow.h"
#include "SDL.h"
#include <string>

class WindowSDL2 : public IWindow
{
public:
	WindowSDL2(const std::string& caption, uint32_t width, uint32_t height, uint32_t posX, uint32_t posY);
	void* GetNativeHandle() override;

private:

	SDL_Window* m_window;
};