#pragma once
#include "util/RefPtr.h"
#include "IFence.h"

#include "vulkan/vulkan.h"

namespace wtv
{
	class VulkanEngine;
	struct VulkanSemaphore : public IReferenceCounted
	{
	public:
		VulkanSemaphore(const VulkanSemaphore& other) = delete;
		VulkanSemaphore(VulkanSemaphore&& other) : m_semaphore(other.m_semaphore), m_engine(other.m_engine)
		{
			other.m_semaphore = VK_NULL_HANDLE;
			other.m_engine = nullptr;
		}
		VulkanSemaphore& operator=(VulkanSemaphore&& other)
		{
			m_semaphore = other.m_semaphore;
			m_engine = other.m_engine;
			other.m_semaphore = nullptr;
			other.m_engine = nullptr;
			return *this;
		}
		VulkanSemaphore(VulkanEngine* engine);
		~VulkanSemaphore();
		VkSemaphore GetNativeHandle() { return m_semaphore; }
		void Signal();
	private:
		VkSemaphore m_semaphore;
		VulkanEngine* m_engine;
	};

	struct VulkanFence : public IFence
	{
	public:
		VulkanFence(const VulkanFence& other) = delete;
		VulkanFence(VulkanEngine* engine, bool createSignaled = false);
		~VulkanFence();
		VkFence GetNativeHandle() { return m_fence; }
		void Wait() const override;
		void Reset() override;
	private:
		VkFence m_fence;
		VulkanEngine* m_engine;
	};
}