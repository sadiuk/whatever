#pragma once
#include "IFramebuffer.h"

#include "vulkan/vulkan.h"
namespace wtv
{
	class VulkanFramebuffer : public IFramebuffer
	{
	public:
		VulkanFramebuffer(const CreateInfo& params, VkDevice device);

	private:
		VkDevice m_device;
		VkFramebuffer m_framebuffer;
	};
}