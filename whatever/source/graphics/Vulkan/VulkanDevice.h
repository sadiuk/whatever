#pragma once
#include "graphics/IDevice.h"
#include "IVulkanSurface.h" 
#include "VulkanGPUImage.h"
#include <ui/IWindow.h>
#include "VulkanQueue.h"
#include "VulkanSwapchain.h"
#include "VulkanCommandPool.h"
#include "VulkanRenderPass.h"
#include "VulkanAllocator.h"
#include "VulkanDebugNamer.h"

#include "vulkan/vulkan.h"
#include "VMAAllocator.h"

#include <optional>
#include <algorithm>
#include <shared_mutex>
#include <unordered_map>

#ifdef _DEBUG
#define USE_VALIDATION_LAYERS true
#else

#endif
namespace wtv
{
	class VulkanDebugNamer;
	class VulkanDevice : public IDevice
	{
		static constexpr uint32_t VULKAN_VERSION = VK_API_VERSION_1_3;

		struct QueueFamilyIndices
		{
			std::optional<uint32_t> graphicsFamilyIndex{};
			std::optional<uint32_t> computeFamilyIndex{};
			std::optional<uint32_t> presentFamilyIndex{};
			bool IsValid() { return graphicsFamilyIndex.has_value() && computeFamilyIndex.has_value() && presentFamilyIndex.has_value(); }
			std::vector<uint32_t> Vector() {
				std::vector<uint32_t> res;
				if (graphicsFamilyIndex.has_value())
					res.push_back(graphicsFamilyIndex.value());
				if (computeFamilyIndex.has_value())
					res.push_back(computeFamilyIndex.value());
				if (presentFamilyIndex.has_value())
					res.push_back(presentFamilyIndex.value());
				auto last = std::unique(res.begin(), res.end()); // remove duplicate indices
				res.erase(last, res.end());
				return res;
			}
		};

	public:
		VulkanDevice(const IDevice::CreationParams& params, IServiceProvider* services, ISurfaceFactory* factory);
		~VulkanDevice();
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
		RefPtr<IGraphicsPipelineLayout> CreateGraphicsPipelineLayout(const GraphicsPipelineLayoutCreateInfo& params) override;
		RefPtr<IGraphicsPipeline> CreateGraphicsPipeline(const IGraphicsPipeline::CreateInfo& params, const RefPtr<const IGraphicsPipelineLayout>& layout) override;
		RefPtr<ICommandBuffer> CreateCommandBuffer() override;
		RefPtr<IGPUImage> GetBackbuffer() override;
		RefPtr<IFence> CreateFence(bool createSignaled) override;
		RefPtr<IGPUBuffer> CreateBuffer(const IGPUBuffer::CreationParams& params, const std::string& name) override;
		ImageFormat GetSwapchainFormat() override;
		RefPtr<IFramebuffer> CreateFramebuffer(IFramebuffer::Properties&& params) override;
		RefPtr<IGPURenderPass> CreateRenderPass(const RenderPassParams& params) override;
		RefPtr<IDescriptorPool> CreateDescriptorPool(const DescriptorPoolParams& params) override;
		RefPtr<IDescriptorSetLayout> CreateDescriptorSetLayout(const DescriptorSetLayoutParams& params) override;
		RefPtr<IBinarySemaphore> CreateBinarySemaphore() const;
		RefPtr<IQueue> GetGraphicsQueue() const override;

		void Present() override;
		void BeginFrame() override;
	public:
		VkDevice GetDevice() const { return m_device; }
		VkPhysicalDevice GetPhysicalDevice() const { return m_physicalDevice; }
		QueueFamilyIndices GetQueueFamilyIndices() { return m_queueFamilyIndices; }
		VulkanQueue* GetGraphicsQueue() { return m_graphicsQueue.get(); }
		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		VulkanRenderPass* ObtainDummyRenderPass(const IFramebuffer::Layout& layout);
		VkInstance GetInstance() { return m_instance; }
		VkPhysicalDevice GetPhysicalDevice() { return m_physicalDevice; }
		VulkanSwapchain* GetSwapchain() { return m_swapchain.get(); }
		const VkPhysicalDeviceMemoryProperties2& GetMemoryProperties() { return m_memoryProperties; }
		int GetMemoryTypeIndex(VkMemoryPropertyFlags memoryPropFlags);
		VulkanSemaphore& GetTimelineSemaphore() { return *(m_gpuTimelineSemaphore.get()); }
		void EnqueueForDeletion(VkBuffer buffer, uint64_t semaphoreWaitValue) { m_buffersToDelete.emplace_back(buffer, semaphoreWaitValue); }
		void FlushDeletions();
		const VulkanDebugNamer& GetDebugNamer() { return m_debugNamer; }
	private:
		VkInstance m_instance;
		VkPhysicalDevice m_physicalDevice;
		VkDevice m_device;
		std::vector<VulkanCommandPool> m_commandPools;
		RefPtr<IVulkanSurface> m_surface;
		RefPtr<VulkanSwapchain> m_swapchain;
		RefPtr<VulkanQueue> m_graphicsQueue;
		RefPtr<VulkanPresentQueue> m_presentQueue;
		RefPtr<VulkanSemaphore> m_gpuTimelineSemaphore;
		RefPtr<VMAAllocator> m_allocator;

		QueueFamilyIndices m_queueFamilyIndices;
		VkPhysicalDeviceMemoryProperties2 m_memoryProperties{};
		CreationParams m_creationParams;

		std::unordered_map<IFramebuffer::Layout, RefPtr<VulkanRenderPass>> m_dummyRPs;
		std::shared_mutex m_dummyRPsMutex;
		std::list<std::pair<VkBuffer, uint64_t>> m_buffersToDelete;

		VulkanDebugNamer m_debugNamer;
	};
}