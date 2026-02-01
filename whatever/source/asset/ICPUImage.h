#pragma once
#include "graphics/IImage.h"
namespace wtv
{
	class ICPUImage : public IImage
	{
		virtual void* GetPixels() = 0;
	};
}