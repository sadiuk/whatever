#pragma once
#include "IEngine.h"
#include "IVulkanSurface.h" 
#include "VulkanGPUImage.h"
#include <ui/IWindow.h>
#include "VulkanQueue.h"
#include "VulkanSwapchain.h"
#include "VulkanCommandPool.h"

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
			std::optional<uint32_t> presentFamilyIndex{};
			bool IsValid() { return graphicsFamilyIndex.has_value() && computeFamilyIndex.has_value() && presentFamilyIndex.has_value(); }
		};

	public:
		VulkanEngine(const IEngine::CreationParams& params, IServiceProvider* services, ISurfaceFactory* factory);
		~VulkanEngine();
		GraphicsAPI GetAPI() override { return Vulkan; }
	private:
		bool Init();
		bool Deinit();
		bool CreateInstance(const std::string& appName);
		bool SelectPhysicalDevice();
		bool CreateLogicalDevice();
		bool CreateAllocator();
		bool InitSurface();
		bool CreateSwapChain();
		bool CreateCommandPools();
		bool CreateCommandQueues();
		bool EnsureValidationLayersAvailable(std::vector<const char*> requestedLayers);
		
	public: // override from IEngine
		RefPtr<IGraphicsPipeline> CreateGraphicsPipeline(const IGraphicsPipeline::CreateInfo& params) override;
		RefPtr<ICommandBuffer> CreateCommandBuffer() override;
		RefPtr<IGPUImage> GetBackbuffer() override;
		ImageFormat GetSwapchainFormat() override;
		void Submit(ICommandBuffer* cb) override;
		void Present() override;
		void BeginFrame() override;
	public:
		RefPtr<IFramebuffer> CreateFramebuffer(const IFramebuffer::CreateInfo& params, VkRenderPass renderpass);
		VkDevice GetDevice() const { return m_device; }
		VkPhysicalDevice GetPhysicalDevice() const { return m_physicalDevice; }
		QueueFamilyIndices GetQueueFamilyIndices() { return m_queueFamilyIndices; }
		VulkanQueue* GetGraphicsQueue() { return m_graphicsQueue.get(); }
	private:
		VkInstance m_instance;
		VkPhysicalDevice m_physicalDevice;
		VkDevice m_device;
		VmaAllocator m_allocator;
		std::vector<VulkanCommandPool> m_commandPools;
		RefPtr<IVulkanSurface> m_surface;
		RefPtr<VulkanSwapchain> m_swapchain;
		RefPtr<VulkanQueue> m_graphicsQueue;
		RefPtr<VulkanPresentQueue> m_presentQueue;

		QueueFamilyIndices m_queueFamilyIndices;
		CreationParams m_creationParams;
	};
}