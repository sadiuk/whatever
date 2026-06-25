#pragma once
#include "IGPUImage.h"
#include "util/RefPtr.h"

#include <glm/glm.hpp>
namespace wtv
{
	struct ISwapchain : public IReferenceCounted
	{
		struct CreateInfo
		{
			glm::uvec2 extent;
		};
		virtual IGPUImage* GetBackBuffer() = 0;
	};
}