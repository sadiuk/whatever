#include "WindowSDL2.h"


WindowSDL2::WindowSDL2(const std::string& caption, uint32_t width, uint32_t height, uint32_t posX, uint32_t posY)
{
	SDL_Init(SDL_INIT_VIDEO);
	m_window = SDL_CreateWindow(caption.c_str(), posX, posY, width, height, SDL_VIDEO_VULKAN);
}

void* WindowSDL2::GetNativeHandle()
{
	return m_window;
}
