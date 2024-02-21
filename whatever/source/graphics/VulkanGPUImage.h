#pragma once
#include "IGPUImage.h"

#include <vulkan/vulkan.h>

#include <vector>
namespace wtv
{
	class VulkanGPUImage : public IGPUImage
	{
		friend class VulkanEngine;
	public:
		VulkanGPUImage(VkDevice device, const CreationParams& params);
		VulkanGPUImage(VkDevice device, const CreationParams& params, VkImage rawHandle);
		~VulkanGPUImage();
		VkImageView GetImageView(const IImage::View& view);
	private:
		VkDevice m_device;
		VkImage m_image;
		std::vector<std::pair<IImage::View, VkImageView>> m_cachedViews; // TODO: hash those image view infos into a single variable
	};
}