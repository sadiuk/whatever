#include "VulkanSync.h"
#include "VkMakros.h"
#include "VulkanEngine.h"
namespace wtv
{
	VulkanSemaphore::VulkanSemaphore(VulkanEngine* engine) : 
		m_engine(engine)
	{
		VkSemaphoreCreateInfo semaphoreCreateInfo{};
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		semaphoreCreateInfo.pNext = nullptr;
		semaphoreCreateInfo.flags = 0;
		ASSERT_VK_SUCCESS(vkCreateSemaphore(m_engine->GetDevice(), &semaphoreCreateInfo, nullptr, &m_semaphore));
	}
	wtv::VulkanSemaphore::~VulkanSemaphore()
	{
		if(m_engine)
			vkDestroySemaphore(m_engine->GetDevice(), m_semaphore, nullptr);
	}
	void VulkanSemaphore::Signal()
	{
		VkSemaphoreSignalInfo signalInfo{};
		signalInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SIGNAL_INFO;
		signalInfo.pNext = nullptr;
		signalInfo.semaphore = m_semaphore;
		//.value
	}
	IServiceProvider* VulkanSemaphore::GetServiceProvider()
	{
		return m_engine->GetServiceProvider();
	}
	VulkanFence::VulkanFence(VulkanEngine* engine, bool createSignaled) : m_engine(engine)
	{
		VkFenceCreateInfo fenceCreateInfo{};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.pNext = nullptr;
		fenceCreateInfo.flags = createSignaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;
		ASSERT_VK_SUCCESS(vkCreateFence(m_engine->GetDevice(), &fenceCreateInfo, nullptr, &m_fence));
	}
	VulkanFence::~VulkanFence()
	{
		vkDestroyFence(m_engine->GetDevice(), m_fence, nullptr);
	}
	void VulkanFence::Wait() const
	{
		vkWaitForFences(m_engine->GetDevice(), 1, &m_fence, VK_TRUE, UINT64_MAX);
	}
	void VulkanFence::Reset()
	{
		vkResetFences(m_engine->GetDevice(), 1, &m_fence);
	}
	IServiceProvider* VulkanFence::GetServiceProvider()
	{
		return m_engine->GetServiceProvider();
	}
}