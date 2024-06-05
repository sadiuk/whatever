#include "VulkanCommandPool.h"
#include "VulkanEngine.h"
#include "VkMakros.h"

#include <cassert>
namespace wtv
{
	wtv::VulkanCommandPool::VulkanCommandPool(VulkanEngine* engine) :
		m_engine(engine)
	{
		assert(m_engine->GetQueueFamilyIndices().graphicsFamilyIndex.value() == m_engine->GetQueueFamilyIndices().computeFamilyIndex.value());
		VkCommandPoolCreateInfo commandPoolInfo{};
		commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolInfo.pNext = nullptr;
		commandPoolInfo.queueFamilyIndex = m_engine->GetQueueFamilyIndices().graphicsFamilyIndex.value();
		commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		ASSERT_VK_SUCCESS_ELSE_RET(vkCreateCommandPool(m_engine->GetDevice(), &commandPoolInfo, nullptr, &m_commandPool));
	}

	RefPtr<VulkanCommandBuffer> wtv::VulkanCommandPool::CreateCommandBuffer()
	{
		auto cb = MakeRef<VulkanCommandBuffer>(m_engine, m_commandPool);
		m_commandBuffers.push_back(cb);
		return cb;
	}

	void wtv::VulkanCommandPool::WaitCommandBuffersAndClear()
	{
		//for (auto& cb : m_commandBuffers)
		//{
		//	uint32_t refCount = cb->RefCount();
		//	assert(refCount == 1);
		//	cb->GetFence().Wait();
		//}
		m_commandBuffers.clear();
		vkResetCommandPool(m_engine->GetDevice(), m_commandPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
	}

	IServiceProvider* VulkanCommandPool::GetServiceProvider()
	{
		return m_engine->GetServiceProvider();
	}

}