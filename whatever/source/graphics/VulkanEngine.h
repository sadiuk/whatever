#pragma once
#include "IEngine.h"
#include "IVulkanSurface.h" 
#include "VulkanGPUImage.h"
#include <ui/IWindow.h>
#include "VulkanQueue.h"

#include "vulkan/vulkan.h"
#include "vma/vk_mem_alloc.h"

#include <optional>

#ifdef _DEBUG
#define USE_VALIDATION_LAYERS true
#else

#endif
namespace wtv
{
	class VulkanEngine : public IEngine
	{
		static constexpr uint32_t VULKAN_VERSION = VK_API_VERSION_1_2;

		struct QueueFamilyIndices
		{
			std::optional<uint32_t> graphicsFamilyIndex{};
			std::optional<uint32_t> computeFamilyIndex{};
			bool IsValid() { return graphicsFamilyIndex.has_value() && computeFamilyIndex.has_value(); }
		};
		struct AvailableSwapchainCapabilities
		{
			VkSurfaceCapabilitiesKHR surfaceCaps;
			std::vector<VkSurfaceFormatKHR> availableFormats;
			std::vector<VkPresentModeKHR> presentModes;
		};
	public:
		VulkanEngine(const IEngine::CreationParams& params, IServiceProvider* services, ISurfaceFactory* factory);
		~VulkanEngine();
		GraphicsAPI GetAPI() override { return Vulkan; }
		VkDevice GetNativeDeviceHandle() { return m_device; }
	private:
		bool Init();
		bool Deinit();
		bool CreateInstance(const std::string& appName);
		bool SelectPhysicalDevice();
		bool CreateLogicalDevice();
		bool CreateAllocator();
		bool InitSurface();
		bool CreateSwapChain();
		bool CreateCommandPool();
		bool CreateCommandQueues();
		bool CreateSwapchainImages(const IImage::CreationParams& params);
		bool EnsureValidationLayersAvailable(std::vector<const char*> requestedLayers);
		AvailableSwapchainCapabilities GetAvailableSwapchainCapabilities();
	public: // override from IEngine
		RefPtr<IGraphicsPipeline> CreateGraphicsPipeline(const IGraphicsPipeline::CreateInfo& params) override;
		RefPtr<ICommandBuffer> CreateCommandBuffer() override;
		std::vector<RefPtr<IGPUImage>> GetSwapchainImages() override;
		ImageFormat GetSwapchainFormat() override;
		void Submit(ICommandBuffer* cb) override;

	public:
		RefPtr<IFramebuffer> CreateFramebuffer(const IFramebuffer::CreateInfo& params, VkRenderPass renderpass);
		VkDevice GetDevice() const { return m_device; }
	private:
		VkInstance m_instance;
		VkPhysicalDevice m_physicalDevice;
		VkDevice m_device;
		VkSwapchainKHR m_swapchain;
		VkFormat m_swapchainFormat;
		VmaAllocator m_allocator;
		VkCommandPool m_commandPool;
		RefPtr<IVulkanSurface> m_surface;

		RefPtr<VulkanQueue> m_graphicsQueue;

		std::vector<RefPtr<VulkanGPUImage>>  m_swapchainImages;


		QueueFamilyIndices m_queueFamilyIndices;
		CreationParams m_creationParams;
	};
}