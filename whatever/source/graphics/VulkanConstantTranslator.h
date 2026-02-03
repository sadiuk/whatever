#pragma once
#include "GraphicsConstants.h"
#include <vulkan/vulkan.h>
#include <type_traits>
namespace wtv
{
	struct VulkanConstantTranslator
	{
		static VkFormat GetVkFormatForAttribute(VertexAtributeType attributeType);
		static VkShaderStageFlagBits GetVkShaderStage(ShaderStage stage);
		static int GetVkShaderStageFlags(ShaderStageFlags stage);
		static VkPrimitiveTopology GetVkPrimitiveTopology(PrimitiveTopology topology);
		static VkFrontFace GetVkFrontFace(FrontFace frontFaceEnum);
		static VkCullModeFlags GetVkCullMode(CullMode cullMode);
		static VkPolygonMode GetVkPolygonMode(PolygonMode polygonModeEnum);
		static VkCompareOp GetCompareOperation(CompareOperation depthTestOperationEnum);
		static VkStencilOp GetStencilTestOperation(StencilTestOperation stencilTestOperationEnum);
		static VkBlendFactor GetVkBlendFactor(BlendFactor blendFactor);
		static VkBlendOp GetVkBlendOp(BlendOperation blendOp);
		static VkFormat GetVkFormat(ImageFormat imageFormat);
		static VkImageType GetVkImageType(ImageDimension dim);
		static ImageFormat GetEngineImageFormat(VkFormat imageFormat);
		static uint32_t GetVkImageUsageMask(std::underlying_type_t<ImageUsage> usageBits);
		static VkBufferUsageFlags GetVkBufferUsageFlags(std::underlying_type_t<BufferUsage> usageBits);
		static VkAttachmentStoreOp GetVkAttachmentStoreOp(AttachmentStoreOp storeOp);
		static VkAttachmentLoadOp GetVkAttachmentLoadOp(AttachmentLoadOp loadOp);
		static VkDescriptorType GetVkDescriptorType(DescriptorType type);
		static VkImageLayout GetVkImageLayout(ImageLayout layout);
	};


}