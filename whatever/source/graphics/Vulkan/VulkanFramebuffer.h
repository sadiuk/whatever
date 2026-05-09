#pragma once
#include "graphics/IFramebuffer.h"
#include "IServiceProvider.h"
#include "VulkanDevice.h"

#include "vulkan/vulkan.h"
namespace wtv
{
	class VulkanFramebuffer : public IFramebuffer, public IServiceProviderHolder
	{
	public:
		VulkanFramebuffer(Properties&& params, VulkanDevice* engine);
		VkFramebuffer GetNativeHandle() { return m_framebuffer; }
		const std::vector<VkImageView>& GetAttachments() const { return m_attachments; }
		IServiceProvider* GetServiceProvider() const override;
		VkFramebuffer GetNativeHandle() const { return m_framebuffer; }
	private:
		VulkanDevice* m_engine;
		VkFramebuffer m_framebuffer;
		std::vector<VkImageView> m_attachments;
		VkRenderPass m_dummyRenderPass;

	};
}