#pragma once
#include <vulkan/vulkan.h>
#include "util/RefPtr.h"
#include "VulkanCommandBuffer.h"
namespace wtv
{
	class VulkanDevice;
	class VulkanCommandPool : public IServiceProviderHolder, public IReferenceCounted
	{
	public:
		VulkanCommandPool(VulkanDevice* engine);
		~VulkanCommandPool();
		RefPtr<VulkanCommandBuffer> CreateCommandBuffer();
		void WaitCommandBuffersAndClear();
		IServiceProvider* GetServiceProvider() const override;
		VkCommandPool GetNativeHandle() { return m_commandPool; }
	private:
		VulkanDevice* m_engine;
		VkCommandPool m_commandPool;
	};
}