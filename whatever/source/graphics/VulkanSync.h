#pragma once
#include "util/RefPtr.h"

#include "vulkan/vulkan.h"

namespace wtv
{
	class VulkanEngine;
	struct VulkanSemaphore : public IReferenceCounted
	{
	public:
		VulkanSemaphore(VulkanEngine* engine);
		~VulkanSemaphore();
		VkSemaphore GetNativeHandle() { return m_semaphore; }
	private:
		VkSemaphore m_semaphore;
		VulkanEngine* m_engine;
	};

	struct VulkanFence : public IReferenceCounted
	{
	public:
		VulkanFence(VulkanEngine* engine);
		~VulkanFence();
		VkFence GetNativeHandle() { return m_fence; }
	private:
		VkFence m_fence;
		VulkanEngine* m_engine;
	};
}