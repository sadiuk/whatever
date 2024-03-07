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
		vkDestroySemaphore(m_engine->GetDevice(), m_semaphore, nullptr);
	}
	VulkanFence::VulkanFence(VulkanEngine* engine) : m_engine(engine)
	{
		VkFenceCreateInfo fenceCreateInfo{};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.pNext = nullptr;
		fenceCreateInfo.flags = 0;
		VkFence fence{};
		ASSERT_VK_SUCCESS(vkCreateFence(m_engine->GetDevice(), &fenceCreateInfo, nullptr, &fence));
	}
	VulkanFence::~VulkanFence()
	{
		vkDestroyFence(m_engine->GetDevice(), m_fence, nullptr);
	}
}