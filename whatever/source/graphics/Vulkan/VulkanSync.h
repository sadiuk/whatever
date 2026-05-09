#pragma once
#include "util/RefPtr.h"
#include "graphics/IFence.h"
#include "graphics/ISemaphore.h"
#include "IServiceProvider.h"

#include "vulkan/vulkan.h"

namespace wtv
{
	class VulkanDevice;
	struct VulkanBinarySemaphore : public IBinarySemaphore, public IServiceProviderHolder
	{
	public:
		VulkanBinarySemaphore(VulkanBinarySemaphore&& other) = default;
		VulkanBinarySemaphore& operator=(VulkanBinarySemaphore&& other) = default;
		VulkanBinarySemaphore(const VulkanBinarySemaphore& other) = delete;
		VulkanBinarySemaphore(const VulkanDevice* engine);
		~VulkanBinarySemaphore();

		VkSemaphore GetNativeHandle() { return m_semaphore; }
		IServiceProvider* GetServiceProvider() const override;

	private:
		VkSemaphore m_semaphore;
		const VulkanDevice* m_engine;
	};

	struct VulkanSemaphore : public ISemaphore, public IServiceProviderHolder
	{
	public:
		VulkanSemaphore(const VulkanSemaphore& other) = delete;
		VulkanSemaphore(VulkanDevice* engine);
		~VulkanSemaphore();
		VkSemaphore GetNativeHandle() { return m_semaphore; }
		uint64_t IncrementValue() { return ++m_value; }
		uint64_t CheckCompletedValue() const;
		IServiceProvider* GetServiceProvider() const override;
	private:
		VkSemaphore m_semaphore;
		VulkanDevice* m_engine;
		std::atomic_uint64_t m_value{ 0 };
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

		IServiceProvider* GetServiceProvider() const override;
	private:
		VkFence m_fence;
		VulkanDevice* m_engine;
	};
}