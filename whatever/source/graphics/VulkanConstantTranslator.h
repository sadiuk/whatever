#pragma once
#include "GraphicsConstants.h"
#include <vulkan/vulkan.h>

struct VulkanConstantsTranslator
{
	static VkFormat GetVkFormatForAttribute(VertexAtributeType attributeType);
	static VkShaderStageFlagBits GetVkShaderStage(ShaderStage stage);
	static VkPrimitiveTopology GetVkPrimitiveTopology(PrimitiveTopology topology);
	static VkFrontFace GetVkFrontFace(FrontFace frontFaceEnum);
	static VkCullModeFlags GetVkCullMode(CullMode cullMode);
	static VkPolygonMode GetVkPolygonMode(PolygonMode polygonModeEnum);
	static VkCompareOp GetCompareOperation(CompareOperation depthTestOperationEnum);
	static VkStencilOp GetStencilTestOperation(StencilTestOperation stencilTestOperationEnum);
	static VkBlendFactor GetVkBlendFactor(BlendFactor blendFactor);
	static VkBlendOp GetVkBlendOp(BlendOperation blendOp);
	static VkFormat GetVkFormat(ImageFormat imageFormat);

	static ImageFormat GetEngineImageFormat(VkFormat imageFormat);
};