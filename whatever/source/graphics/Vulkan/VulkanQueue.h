#pragma once
#include "graphics/IQueue.h"
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
		void Submit(const QueueSubmitInfo& sbmtInfo) override;
		IFence& GetFence() override;
		VulkanBinarySemaphore& GetRenderFinishedSemaphore() { return *m_signalSemaphore.get(); }
		VkQueue GetNativeHandle() { return m_queue; }
	protected:
		VkQueue m_queue;
		VkDevice m_deviceRaw;
		RefPtr<VulkanBinarySemaphore> m_signalSemaphore;
		VulkanFence m_fence;
		VulkanDevice* m_device;
	};
	class VulkanPresentQueue : public VulkanQueue
	{
	public:
		VulkanPresentQueue(VulkanDevice* engine, uint32_t queueFamilyIndex, uint32_t queueIndex) :
			VulkanQueue(engine, queueFamilyIndex, queueIndex) {}
		void PresentSwapchainImage(VulkanSwapchain* swapchain, VulkanQueue* waitQueue);
	};
}
