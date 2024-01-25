#pragma once
#include "GraphicsConstants.h"
#include <vulkan/vulkan.h>

struct VulkanConstantsTranslator
{
	static VkFormat GetVkFormatForAttribute(VertexAtributeType attributeType);
	static VkShaderStageFlagBits GetVkShaderStage(ShaderStage stage);
	static VkPrimitiveTopology GetVkPrimitiveTopology(PrimitiveTopology topology);
	static VkFrontFace GetVkFrontFace(FrontFace frontFaceEnum);
};