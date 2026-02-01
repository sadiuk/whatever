#pragma once
#include "ISwapchain.h"
#include "IVulkanSurface.h"
#include "VulkanGPUImage.h"

namespace wtv
{
	class VulkanDevice;
	class VulkanSwapchain : public ISwapchain, public IServiceProviderHolder
	{
	public:
		VulkanSwapchain(VulkanDevice* engine, IVulkanSurface* surface);
		~VulkanSwapchain();
		RefPtr<IGPUImage> GetBackBuffer() override;
		VkSwapchainKHR GetNativeHandle() { return m_swapchain; }
		uint32_t GetImageIndex() { return m_imageIndex; }
		void GetNextImage();
		VkFormat GetFormat() { return m_format; }
		uint32_t GetImageCount() { return (uint32_t)m_images.size(); }

		IServiceProvider* GetServiceProvider() override;
	private:
		VulkanDevice* m_engine;
		IVulkanSurface* m_surface;
		VkFormat m_format;
		VkSwapchainKHR m_swapchain;
		VkDevice m_device;
		std::vector<RefPtr<VulkanGPUImage>> m_images;
		uint32_t m_imageIndex = 0;
		struct AvailableSwapchainCapabilities
		{
			VkSurfaceCapabilitiesKHR surfaceCaps;
			std::vector<VkSurfaceFormatKHR> availableFormats;
			std::vector<VkPresentModeKHR> presentModes;
		};
		AvailableSwapchainCapabilities GetAvailableSwapchainCapabilities();
		void CreateImages(const IImage::CreationParams& params);
	};
}