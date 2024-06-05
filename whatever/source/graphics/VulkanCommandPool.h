#pragma once
#include "VulkanCommandBuffer.h"
#include <vulkan/vulkan.h>
namespace wtv
{
	class VulkanEngine;
	class VulkanCommandPool : public IServiceProviderHolder
	{
	public:
		VulkanCommandPool(VulkanEngine* engine);
		RefPtr<VulkanCommandBuffer> CreateCommandBuffer();
		void WaitCommandBuffersAndClear();
		IServiceProvider* GetServiceProvider() override;
	private:
		VulkanEngine* m_engine;
		std::vector<RefPtr<VulkanCommandBuffer>> m_commandBuffers;
		VkCommandPool m_commandPool;
	};
}