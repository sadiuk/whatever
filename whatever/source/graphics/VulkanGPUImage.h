#pragma once
#include "IGPUImage.h"

#include <vulkan/vulkan.h>
namespace wtv
{
	class VulkanGPUImage : public IGPUImage
	{
		friend class VulkanEngine;
	private:
		VulkanGPUImage(VkDevice device, const CreationParams& params);
		VkDevice m_device;
		VkImage m_image;
	};
}