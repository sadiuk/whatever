#pragma once
#include "IServiceProvider.h"
#include "graphics/IGPUImage.h"

#include "IVulkanAllocator.h"

#include <vulkan/vulkan.h>

#include <vector>
namespace wtv
{
	class VulkanGPUImage : public IGPUImage, public IServiceProviderHolder
	{
		friend class VulkanDevice;
	public:
		VulkanGPUImage(VulkanDevice* engine, const CreationParams& params, MemoryPropertyFlags flags, const std::string& name, IVulkanAllocator* allocator);
		VulkanGPUImage(VulkanDevice* engine, const CreationParams& params, const std::string& name, VkImage rawHandle);
		~VulkanGPUImage();
		VkImage GetNativeHandle() const { return m_image; }
		VkImageView GetImageView(const IImage::View& view);
		IServiceProvider* GetServiceProvider() const override;
	private:
		std::string m_debugName;
		VulkanDevice* m_device;
		VkImage m_image;
		VkDeviceMemory m_memory;
		IVulkanAllocator* m_allocator {};
		std::vector<std::pair<IImage::View, VkImageView>> m_cachedViews{ }; // TODO: hash those image view infos into a single variable
	};
}