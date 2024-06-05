#pragma once
#include "IGPUImage.h"
#include "IServiceProvider.h"

#include <vulkan/vulkan.h>

#include <vector>
namespace wtv
{
	class VulkanGPUImage : public IGPUImage, public IServiceProviderHolder
	{
		friend class VulkanEngine;
	public:
		VulkanGPUImage(VulkanEngine* engine, const CreationParams& params);
		VulkanGPUImage(VulkanEngine* engine, const CreationParams& params, VkImage rawHandle);
		~VulkanGPUImage();
		VkImageView GetImageView(const IImage::View& view);
		IServiceProvider* GetServiceProvider() override;
	private:
		VulkanEngine* m_engine;
		VkImage m_image;
		std::vector<std::pair<IImage::View, VkImageView>> m_cachedViews; // TODO: hash those image view infos into a single variable
	};
}