#include "WindowSDL2.h"

namespace wtv
{
	WindowSDL2::WindowSDL2(const CreationParams& params)
	{
		SDL_Init(SDL_INIT_VIDEO);
		m_window = SDL_CreateWindow(params.caption.c_str(),
			params.windowPos.x,
			params.windowPos.y,
			params.size.x,
			params.size.y,
			SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN);
	}

	void* WindowSDL2::GetNativeHandle()
	{
		return m_window;
	}

	bool WindowSDL2::IsOpen()
	{
		return true;
	}
}