#pragma once
#include "IQueue.h"
#include "VulkanSync.h"
#include "VulkanSwapchain.h"

#include "vulkan/vulkan.h"

namespace wtv
{
	class VulkanDevice;
	class VulkanQueue : public IQueue
	{
	public:
		VulkanQueue(VulkanDevice* engine, uint32_t queueFamilyIndex, uint32_t queueIndex);
		~VulkanQueue() {}
		void Submit(ICommandBuffer* cmdBuffer) override;
		void Submit(ICommandBuffer* cmdBuffer, IQueue* waitQueue) override;
		IFence& GetFence() override;
		VulkanSemaphore& GetRenderFinishedSemaphore() { return m_signalSemaphore; }
		VkQueue GetNativeHandle() { return m_queue; }
	protected:
		VkQueue m_queue;
		VkDevice m_device;
		VulkanSemaphore m_signalSemaphore;
		VulkanFence m_fence;
		VulkanDevice* m_engine;
	};
	class VulkanPresentQueue : public VulkanQueue
	{
	public:
		VulkanPresentQueue(VulkanDevice* engine, uint32_t queueFamilyIndex, uint32_t queueIndex) :
			VulkanQueue(engine, queueFamilyIndex, queueIndex) {}
		void PresentSwapchainImage(VulkanSwapchain* swapchain, VulkanQueue* waitQueue);
	};
}
