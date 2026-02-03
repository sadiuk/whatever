#pragma once
#include "vulkan/vulkan.h"
#include "IGPURenderPass.h"

namespace wtv
{
	class VulkanDevice;
	class VulkanRenderPass : public IGPURenderPass
	{
		VkRenderPass m_renderPass;
		VulkanDevice* m_device;
		IFramebuffer::Layout m_fbLayout;
	public:
		VulkanRenderPass(VulkanDevice* device, const IFramebuffer::Layout& params);
		~VulkanRenderPass();
		VkRenderPass GetNativeHandle() const
		{
			return m_renderPass;
		}
	};
}