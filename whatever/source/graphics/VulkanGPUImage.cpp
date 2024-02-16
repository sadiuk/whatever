#include "VulkanGPUImage.h"
#include "VulkanConstantTranslator.h"
#include "VkMakros.h"
namespace wtv
{
	VulkanGPUImage::VulkanGPUImage(VkDevice device, const CreationParams& params) :
		IGPUImage(params),
		m_device(device)
	{
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.pNext = nullptr;
		imageInfo.imageType = VulkanConstantTranslator::GetVkImageType(params.dimension);
		imageInfo.format = VulkanConstantTranslator::GetVkFormat(params.format);
		imageInfo.extent.width = params.width;
		imageInfo.extent.height = params.height;
		imageInfo.extent.depth = params.depth;
		imageInfo.mipLevels = params.mipLevels;
		imageInfo.arrayLayers = params.arraySize;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.usage = VulkanConstantTranslator::GetVkImageUsageMask(params.usageFlags);
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.queueFamilyIndexCount = 0;
		imageInfo.pQueueFamilyIndices = nullptr;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_GENERAL;

		ASSERT_VK_SUCCESS(vkCreateImage(m_device, &imageInfo, nullptr, &m_image));
	}
}