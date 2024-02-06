#pragma once
#include <glm/glm.hpp>
namespace wtv
{
	struct ISwapChain
	{
		struct CreateInfo
		{
			glm::uvec2 extent;
		};
	};
}