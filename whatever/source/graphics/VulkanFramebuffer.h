#pragma once
#include "IFramebuffer.h"
#include "vulkan/vulkan.h"
namespace wtv
{
	class VulkanFramebuffer : public IFramebuffer
	{
	public:
		VulkanFramebuffer(const CreateInfo& params, VkDevice device, VkRenderPass renderpass);
		VkFramebuffer GetNativeHandle() { return m_framebuffer; }
		const std::vector<VkImageView>& GetAttachments() const { return m_attachments; }
	private:
		VkDevice m_device;
		VkFramebuffer m_framebuffer;
		std::vector<VkImageView> m_attachments;

	};
}