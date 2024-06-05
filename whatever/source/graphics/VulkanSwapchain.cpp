#pragma once
#include "VulkanSwapchain.h"
#include "VulkanConstantTranslator.h"
#include "VulkanEngine.h"
#include "VkMakros.h"

#include "vulkan/vulkan.h"

namespace wtv
{
	VulkanSwapchain::VulkanSwapchain(VulkanEngine* engine, IVulkanSurface* surface)
		: m_engine(engine), m_surface(surface), m_device(engine->GetDevice())
	{
		VkSwapchainCreateInfoKHR swapchainCreateInfo{};
		auto queueFamilyIndices = m_engine->GetQueueFamilyIndices();

		AvailableSwapchainCapabilities caps = GetAvailableSwapchainCapabilities();
		VkSurfaceFormatKHR surfaceFmt{};

		IImage::CreationParams swapchainImageParams{};

		if (auto surfaceFmtIter = std::find_if(
			caps.availableFormats.begin(), caps.availableFormats.end(),
			[](const VkSurfaceFormatKHR& fmt) { return fmt.format == VK_FORMAT_B8G8R8A8_SRGB; });
			surfaceFmtIter != caps.availableFormats.end())
		{
			surfaceFmt = *surfaceFmtIter;
		}
		else
		{
			assert(false);
			surfaceFmt = caps.availableFormats[0];
		}
		swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCreateInfo.pNext = nullptr;
		swapchainCreateInfo.flags = 0;
		swapchainCreateInfo.surface = (VkSurfaceKHR)m_surface->GetNativeHandle();
		swapchainCreateInfo.minImageCount = 3;
		swapchainCreateInfo.imageFormat = m_format = surfaceFmt.format;
		swapchainCreateInfo.imageColorSpace = surfaceFmt.colorSpace;
		swapchainCreateInfo.imageExtent = caps.surfaceCaps.currentExtent;
		swapchainCreateInfo.imageArrayLayers = 1;
		swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		swapchainImageParams.width = swapchainCreateInfo.imageExtent.width;
		swapchainImageParams.height = swapchainCreateInfo.imageExtent.height;
		swapchainImageParams.arraySize = swapchainCreateInfo.imageArrayLayers;
		swapchainImageParams.format = VulkanConstantTranslator::GetEngineImageFormat(swapchainCreateInfo.imageFormat);

		if (queueFamilyIndices.graphicsFamilyIndex.value() != queueFamilyIndices.computeFamilyIndex.value())
		{
			uint32_t queuefamilyIndices[2] = {
				queueFamilyIndices.graphicsFamilyIndex.value(),
				queueFamilyIndices.computeFamilyIndex.value()
			};
			swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			swapchainCreateInfo.queueFamilyIndexCount = 2;
			swapchainCreateInfo.pQueueFamilyIndices = queuefamilyIndices;
		}
		else
		{
			uint32_t queueFamilyIndex = queueFamilyIndices.graphicsFamilyIndex.value();
			swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			swapchainCreateInfo.queueFamilyIndexCount = 1;
			swapchainCreateInfo.pQueueFamilyIndices = &queueFamilyIndex;
		}
		swapchainCreateInfo.preTransform = caps.surfaceCaps.currentTransform;
		swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainCreateInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
		swapchainCreateInfo.clipped = VK_TRUE;
		swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

		ASSERT_VK_SUCCESS(vkCreateSwapchainKHR(m_engine->GetDevice(), &swapchainCreateInfo, nullptr, &m_swapchain));

		CreateImages(swapchainImageParams);
	}
	VulkanSwapchain::~VulkanSwapchain()
	{
		vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
	}
	RefPtr<IGPUImage> VulkanSwapchain::GetBackBuffer()
	{
		return m_images[m_imageIndex].get();
	}
	VulkanSwapchain::AvailableSwapchainCapabilities VulkanSwapchain::GetAvailableSwapchainCapabilities()
	{
		AvailableSwapchainCapabilities caps{};
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_engine->GetPhysicalDevice(), (VkSurfaceKHR)m_surface->GetNativeHandle(), &caps.surfaceCaps);

		uint32_t formatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_engine->GetPhysicalDevice(), (VkSurfaceKHR)m_surface->GetNativeHandle(), &formatCount, nullptr);
		caps.availableFormats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_engine->GetPhysicalDevice(), (VkSurfaceKHR)m_surface->GetNativeHandle(), &formatCount, caps.availableFormats.data());

		uint32_t modeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(m_engine->GetPhysicalDevice(), (VkSurfaceKHR)m_surface->GetNativeHandle(), &modeCount, nullptr);
		caps.presentModes.resize(modeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(m_engine->GetPhysicalDevice(), (VkSurfaceKHR)m_surface->GetNativeHandle(), &modeCount, caps.presentModes.data());
		return caps;
	}
	void VulkanSwapchain::CreateImages(const IImage::CreationParams& params)
	{
		uint32_t imageCount{};
		ASSERT_VK_SUCCESS_ELSE_RET(vkGetSwapchainImagesKHR(m_device, m_swapchain, &imageCount, nullptr));
		std::vector<VkImage> rawSwapchainImages;
		rawSwapchainImages.resize(imageCount);
		ASSERT_VK_SUCCESS_ELSE_RET(vkGetSwapchainImagesKHR(m_device, m_swapchain, &imageCount, rawSwapchainImages.data()));

		m_images.resize(imageCount);
		for (int i = 0; i < m_images.size(); ++i)
		{
			m_images[i] = MakeRef<VulkanGPUImage>(m_engine, params, rawSwapchainImages[i]);
		}
	}
	void VulkanSwapchain::GetNextImage()
	{
		VulkanFence acquireImageFence(m_engine);
		ASSERT_VK_SUCCESS_ELSE_RET(vkAcquireNextImageKHR(m_device, m_swapchain, UINT64_MAX, VK_NULL_HANDLE, acquireImageFence.GetNativeHandle(), &m_imageIndex));
		acquireImageFence.Wait();
	}
	IServiceProvider* VulkanSwapchain::GetServiceProvider()
	{
		return m_engine->GetServiceProvider();
	}
}