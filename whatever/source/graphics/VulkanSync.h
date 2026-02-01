#pragma once
#include "util/RefPtr.h"
#include "IFence.h"
#include "IServiceProvider.h"

#include "vulkan/vulkan.h"

namespace wtv
{
	class VulkanDevice;
	struct VulkanSemaphore : public IReferenceCounted, public IServiceProviderHolder
	{
	public:
		VulkanSemaphore(const VulkanSemaphore& other) = delete;
		VulkanSemaphore(VulkanSemaphore&& other) : m_semaphore(other.m_semaphore), m_engine(other.m_engine)
		{
			other.m_semaphore = VK_NULL_HANDLE;
			other.m_engine = nullptr;
		}
		VulkanSemaphore& operator=(VulkanSemaphore&& other) noexcept
		{
			m_semaphore = other.m_semaphore;
			m_engine = other.m_engine;
			other.m_semaphore = nullptr;
			other.m_engine = nullptr;
			return *this;
		}
		VulkanSemaphore(VulkanDevice* engine);
		~VulkanSemaphore();
		VkSemaphore GetNativeHandle() { return m_semaphore; }
		void Signal();

		IServiceProvider* GetServiceProvider() override;
	private:
		VkSemaphore m_semaphore;
		VulkanDevice* m_engine;
	};

	struct VulkanFence : public IFence, public IServiceProviderHolder
	{
	public:
		VulkanFence(const VulkanFence& other) = delete;
		VulkanFence(VulkanDevice* engine, bool createSignaled = false);
		~VulkanFence();
		VkFence GetNativeHandle() { return m_fence; }
		void Wait() const override;
		void Reset() override;

		IServiceProvider* GetServiceProvider() override;
	private:
		VkFence m_fence;
		VulkanDevice* m_engine;
	};
}