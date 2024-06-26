#include "VulkanFramebuffer.h"
#include "VulkanGPUImage.h"
#include "VkMakros.h"

namespace wtv
{
	VulkanFramebuffer::VulkanFramebuffer(const CreateInfo& params, VulkanEngine* engine, VkRenderPass renderpass) :
		IFramebuffer(params),
		m_engine(engine)
	{
		m_attachments.reserve(params.colorBuffers.size() + params.depthBuffers.size());
		bool extentInitialized = false;
		uint32_t width, height, arraySize;
		for (auto colorBuf : params.colorBuffers)
		{
			VulkanGPUImage* img = reinterpret_cast<VulkanGPUImage*>(colorBuf.image);
			m_attachments.push_back(img->GetImageView(colorBuf));
			if (!extentInitialized)
			{
				const auto& props = img->GetProperties();
				width = props.width;
				height = props.height;
				arraySize = props.arraySize;
				extentInitialized = true;
			}
		}
		for (auto depthBuf : params.depthBuffers)
		{
			VulkanGPUImage* img = reinterpret_cast<VulkanGPUImage*>(&depthBuf.image);
			m_attachments.push_back(img->GetImageView(depthBuf));
			if (!extentInitialized)
			{
				const auto& props = img->GetProperties();
				width = props.width;
				height = props.height;
				arraySize = props.arraySize;
				extentInitialized = true;
			}
		}
		assert(extentInitialized);
		VkFramebufferCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.renderPass = VK_NULL_HANDLE;
		createInfo.width = width;
		createInfo.height = height;
		createInfo.layers = arraySize;
		createInfo.attachmentCount = (uint32_t)m_attachments.size();
		createInfo.pAttachments = m_attachments.data();
		createInfo.renderPass = renderpass;
		ASSERT_VK_SUCCESS(vkCreateFramebuffer(m_engine->GetDevice(), &createInfo, nullptr, &m_framebuffer));
	}
	IServiceProvider* VulkanFramebuffer::GetServiceProvider()
	{
		return m_engine->GetServiceProvider();
	}
}