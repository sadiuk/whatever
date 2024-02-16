#pragma once
#include "GraphicsConstants.h"

#include <cstdint>
#include <type_traits>
namespace wtv
{
	struct IGPUImage
	{
		struct View
		{
		};
		struct CreationParams
		{
			uint32_t width;
			uint32_t height;
			uint32_t depth = 1;
			uint32_t arraySize = 1;
			uint32_t mipLevels = 1;
			ImageFormat format;
			ImageDimension dimension = ImageDimension::Dimension2D;
			uint32_t samples = 1;
			std::underlying_type_t<ImageUsage> usageFlags;
		};

	protected:
		IGPUImage(const CreationParams& params) : m_params(params) {}
		CreationParams m_params;
	};
}