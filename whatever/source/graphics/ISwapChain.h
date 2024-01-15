#pragma once
#include <glm/glm.hpp>

struct ISwapChain
{
	struct CreateInfo
	{
		glm::uvec2 extent;
	};
};