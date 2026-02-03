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
	public:
		VulkanRenderPass(VulkanDevice* device, const RenderPassParams& params);
		~VulkanRenderPass();
		VkRenderPass GetNativeHandle() const
		{
			return m_renderPass;
		}
		const RenderPassParams& GetProperties() const override
		{
			return m_properties;
		}
	private:
		RenderPassParams m_properties;
	};
}