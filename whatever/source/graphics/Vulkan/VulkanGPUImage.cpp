#include "VulkanGPUImage.h"
#include "VulkanConstantTranslator.h"
#include "VkMakros.h"
#include "VulkanDevice.h"
#include "graphics/FormatUtils.h"
namespace wtv
{
	VulkanGPUImage::VulkanGPUImage(VulkanDevice* engine, const CreationParams& params, MemoryPropertyFlags flags, const std::string& name, IVulkanAllocator* allocator) :
		IGPUImage(params),
		m_device(engine),
		m_allocator(allocator)
	{
		m_image = m_allocator->AllocateImage(params, flags);
		m_device->GetDebugNamer().SetResourceName(m_image, VK_OBJECT_TYPE_IMAGE, name.c_str());
		m_debugName = name;
	}

	VulkanGPUImage::VulkanGPUImage(VulkanDevice* engine, const CreationParams& params, const std::string& name, VkImage rawHandle) :
		IGPUImage(params),
		m_device(engine),
		m_image(rawHandle)
	{
		m_debugName = name;
		m_device->GetDebugNamer().SetResourceName(m_image, VK_OBJECT_TYPE_IMAGE, name.c_str());
	}

	VulkanGPUImage::~VulkanGPUImage()
	{
		uint64_t semaphoreWaitValue = GetSemaphoreWaitValue();
		IVulkanAllocator* allocator = m_allocator;
		VkDevice device = m_device->GetDevice();
		for (auto view : m_cachedViews)
		{
			VkImageView imageView = view.second;
			m_device->EnqueueForDeletion([semaphoreWaitValue, imageView, allocator, device](uint64_t completedValue) 
			{
				if (completedValue >= semaphoreWaitValue)
				{
					vkDestroyImageView(device, imageView, nullptr);
					return true;
				}
				return false;
			});
		}

		m_device->EnqueueForDeletion([semaphoreWaitValue, allocator, device, image = m_image](uint64_t completedValue)
		{
			if (completedValue >= semaphoreWaitValue)
			{
				if(allocator)
					allocator->DeallocateImage(image);
				return true;
			}
			return false;
		});

	}

	VkImageView VulkanGPUImage::GetImageView(const IImage::View& view)
	{
		assert(view.image == this);
		if (view.image != this) return VK_NULL_HANDLE;

		if (auto it = std::find_if(m_cachedViews.begin(), m_cachedViews.end(), 
			[&](const std::pair<IImage::View, VkImageView>& cachedView) {
				return view == cachedView.first;
			});
		it != m_cachedViews.end())
		{
			return it->second;
		}
		VkImageView newImageView;
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.pNext = nullptr;
		viewInfo.flags = 0;
		viewInfo.image = m_image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = VulkanConstantTranslator::GetVkFormat(m_props.format);
		viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewInfo.subresourceRange.baseArrayLayer = view.baseArrayLayer;
		viewInfo.subresourceRange.baseMipLevel = view.baseMip;
		viewInfo.subresourceRange.layerCount = view.arrayLayerCount;
		viewInfo.subresourceRange.levelCount = view.mipCount;
		viewInfo.subresourceRange.aspectMask = VulkanConstantTranslator::GetVkImageAspectFlagBits(view.aspectFlags);
		
		static int a = 0;
		ASSERT_VK_SUCCESS_ELSE_RET0(vkCreateImageView(m_device->GetDevice(), &viewInfo, nullptr, &newImageView));
		a++;
		m_device->GetDebugNamer().SetResourceName(newImageView, VK_OBJECT_TYPE_IMAGE_VIEW, (m_debugName + " Cached Image View" + std::to_string(a)).c_str());
		m_cachedViews.push_back({ view, newImageView });
		return newImageView;
	}

	IServiceProvider* VulkanGPUImage::GetServiceProvider() const
	{
		return m_device->GetServiceProvider();
	}
}