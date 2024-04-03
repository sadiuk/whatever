#pragma once
#include "util/RefPtr.h"

#include "vulkan/vulkan.h"

namespace wtv
{
	class VulkanEngine;
	struct VulkanSemaphore : public IReferenceCounted
	{
	public:
		VulkanSemaphore(const VulkanSemaphore& other) = delete;
		VulkanSemaphore(VulkanEngine* engine);
		~VulkanSemaphore();
		VkSemaphore GetNativeHandle() { return m_semaphore; }
		void Signal();
	private:
		VkSemaphore m_semaphore;
		VulkanEngine* m_engine;
	};

	struct VulkanFence : public IReferenceCounted
	{
	public:
		VulkanFence(const VulkanFence& other) = delete;
		VulkanFence(VulkanEngine* engine);
		~VulkanFence();
		VkFence GetNativeHandle() { return m_fence; }
		void Wait() const;
		void Reset();
	private:
		VkFence m_fence;
		VulkanEngine* m_engine;
	};
}