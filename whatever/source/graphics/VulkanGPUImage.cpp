#include "VulkanGPUImage.h"
#include "VulkanConstantTranslator.h"
#include "VkMakros.h"
#include "VulkanEngine.h"
#include "FormatUtils.h"
namespace wtv
{
	VulkanGPUImage::VulkanGPUImage(VulkanEngine* engine, const CreationParams& params) :
		IGPUImage(params),
		m_engine(engine)	
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

		ASSERT_VK_SUCCESS(vkCreateImage(m_engine->GetDevice(), &imageInfo, nullptr, &m_image));

		VkMemoryRequirements memRequirements{};
		vkGetImageMemoryRequirements(m_engine->GetDevice(), m_image, &memRequirements);


		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.pNext = nullptr;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = m_engine->FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		ASSERT_VK_SUCCESS(vkAllocateMemory(m_engine->GetDevice(), &allocInfo, nullptr, &m_memory));

		ASSERT_VK_SUCCESS(vkBindImageMemory(m_engine->GetDevice(), m_image, m_memory, 0));
	}
	VulkanGPUImage::VulkanGPUImage(VulkanEngine* engine, const CreationParams& params, VkImage rawHandle) :
		IGPUImage(params),
		m_engine(engine),
		m_image(rawHandle)
	{
	}
	VulkanGPUImage::~VulkanGPUImage()
	{
		for (auto view : m_cachedViews)
		{
			vkDestroyImageView(m_engine->GetDevice(), view.second, nullptr);
		}
		vkDestroyImage(m_engine->GetDevice(), m_image, nullptr);
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
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

		ASSERT_VK_SUCCESS_ELSE_RET0(vkCreateImageView(m_engine->GetDevice(), &viewInfo, nullptr, &newImageView));

		m_cachedViews.push_back({ view, newImageView });

		return newImageView;
	}
	IServiceProvider* VulkanGPUImage::GetServiceProvider()
	{
		return m_engine->GetServiceProvider();
	}
}