#pragma once
#include "IWindow.h"
#include "SDL.h"
#include <string>
namespace wtv
{
	class WindowSDL2 : public IWindow
	{
	public:
		WindowSDL2(const CreationParams& params);
		void* GetNativeHandle() override;
		bool IsOpen() override;
		void Update() override;
		glm::ivec2 GetMousePosition() override;
	private:
		bool m_keepOpen = true;
		SDL_Window* m_window;
	};
}