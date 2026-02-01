#pragma once
#include "VulkanCommandBuffer.h"
#include <vulkan/vulkan.h>
namespace wtv
{
	class VulkanDevice;
	class VulkanCommandPool : public IServiceProviderHolder
	{
	public:
		VulkanCommandPool(VulkanDevice* engine);
		RefPtr<VulkanCommandBuffer> CreateCommandBuffer();
		void WaitCommandBuffersAndClear();
		IServiceProvider* GetServiceProvider() override;
	private:
		VulkanDevice* m_engine;
		std::vector<RefPtr<VulkanCommandBuffer>> m_commandBuffers;
		VkCommandPool m_commandPool;
	};
}