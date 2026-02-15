#include "WindowSDL2.h"
#include "imgui_impl_sdl2.h"
#include "SDL2ConstantsTranslator.h"

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
		return m_keepOpen;
	}
	void WindowSDL2::Update()
	{
		SDL_Event e;
		while (SDL_PollEvent(&e) > 0)
		{
			ImGui_ImplSDL2_ProcessEvent(&e);

			switch (e.type)
			{
			case SDL_QUIT:
				m_keepOpen = false;
				NotifyWindowClose();
				break;
			case SDL_MOUSEMOTION:
				NotifyMouseMove(glm::ivec2(e.motion.x, e.motion.y));
				break;
			case SDL_MOUSEBUTTONDOWN:
				NotifyMouseButtonStateChange(SDL2ConstantsTranslator::GetMouseButton(e.button.button), MouseButtonState::Pressed);
				break;
			case SDL_MOUSEBUTTONUP:
				NotifyMouseButtonStateChange(SDL2ConstantsTranslator::GetMouseButton(e.button.button), MouseButtonState::Released);
				break;
			case SDL_KEYDOWN:
				NotifyKeyStateChange(SDL2ConstantsTranslator::GetKey(e.key.keysym.sym), KeyState::Pressed);
				break;
			case SDL_KEYUP:
				NotifyKeyStateChange(SDL2ConstantsTranslator::GetKey(e.key.keysym.sym), KeyState::Released);
				break;

			}


			SDL_UpdateWindowSurface(m_window);

		}
	}
	glm::ivec2 WindowSDL2::GetMousePosition()
	{
		glm::ivec2 mousePos;
		SDL_GetMouseState(&mousePos.x, &mousePos.y);
		return mousePos;
	}
}