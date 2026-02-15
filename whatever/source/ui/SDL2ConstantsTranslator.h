#pragma once
#include "WindowConstants.h"
#include "SDL2/SDL_mouse.h"
namespace wtv
{
	struct SDL2ConstantsTranslator
	{
		static MouseButton GetMouseButton(int sdlMouseButton);
		static Key GetKey(int sdlKeycode);
	};
}