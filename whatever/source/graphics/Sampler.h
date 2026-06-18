#pragma once
#include "GraphicsConstants.h"
#include "glm/glm.hpp"
namespace wtv
{
	struct SamplerCreateInfo
	{
        SamplerFilter magFilter;
        SamplerFilter minFilter;
        SamplerMipMapMode mipmapMode;
        SamplerAddressMode addressModeU;
        SamplerAddressMode addressModeV;
        SamplerAddressMode addressModeW;
        float mipLodBias = 0;
        bool anisotropyEnable = false;
        float maxAnisotropy = 1.0f;
        bool compareEnable = false;
        CompareOperation compareOp = CompareOperation::Never;
        float minLod = 0.0f;
        float maxLod = 0.0f;
        bool unnormalizedCoordinates = false;
	};
}