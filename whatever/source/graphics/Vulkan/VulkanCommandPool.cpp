#include "VulkanCommandPool.h"
#include "VulkanDevice.h"
#include "VkMakros.h"
#include "VulkanCommandBuffer.h"
#include <cassert>
namespace wtv
{
	wtv::VulkanCommandPool::VulkanCommandPool(VulkanDevice* engine) :
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

	VulkanCommandPool::~VulkanCommandPool()
	{
		vkDestroyCommandPool(m_engine->GetDevice(), m_commandPool, nullptr);
	}

	RefPtr<VulkanCommandBuffer> wtv::VulkanCommandPool::CreateCommandBuffer()
	{
		auto cb = MakeRef<VulkanCommandBuffer>(m_engine, RefPtr(this));
		return cb;
	}

	void wtv::VulkanCommandPool::WaitCommandBuffersAndClear()
	{
		vkResetCommandPool(m_engine->GetDevice(), m_commandPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
	}

	IServiceProvider* VulkanCommandPool::GetServiceProvider() const
	{
		return m_engine->GetServiceProvider();
	}

}