#pragma once
#include "IFramebuffer.h"
#include "IServiceProvider.h"
#include "VulkanEngine.h"

#include "vulkan/vulkan.h"
namespace wtv
{
	class VulkanFramebuffer : public IFramebuffer, public IServiceProviderHolder
	{
	public:
		VulkanFramebuffer(const CreateInfo& params, VulkanEngine* engine, VkRenderPass renderpass);
		VkFramebuffer GetNativeHandle() { return m_framebuffer; }
		const std::vector<VkImageView>& GetAttachments() const { return m_attachments; }
		IServiceProvider* GetServiceProvider() override;
	private:
		VulkanEngine* m_engine;
		VkFramebuffer m_framebuffer;
		std::vector<VkImageView> m_attachments;

	};
}