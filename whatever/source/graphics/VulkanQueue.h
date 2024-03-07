#pragma once
#include "IQueue.h"
#include "VulkanSync.h"

#include "vulkan/vulkan.h"

namespace wtv
{
	class VulkanEngine;
	class VulkanQueue : public IQueue
	{
	public:
		VulkanQueue(VulkanEngine* engine, uint32_t queueFamilyIndex, uint32_t queueIndex);
		~VulkanQueue() {}
		void Submit(ICommandBuffer* cmdBuffer) override;
	private:
		VkQueue m_queue;
		VkDevice m_device;
		VulkanSemaphore m_signalSemaphore;
		VulkanFence m_fence;
		VulkanEngine* m_engine;
	};
}
