#pragma once
#include <string>

#include "glm/glm.hpp"


namespace wtv
{
	class IWindow
	{
	public:
		enum WindowLib
		{
			SDL2
		};
		struct CreationParams
		{
			std::string caption;
			glm::uvec2 size;
			glm::ivec2 windowPos;
		};
		virtual bool IsOpen() = 0;
		virtual void* GetNativeHandle() = 0;
		virtual WindowLib GetWindowingLib() = 0;
	};
}