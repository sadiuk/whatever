#include "VulkanFramebuffer.h"
#include "VulkanGPUImage.h"
#include "VkMakros.h"
#include "FormatUtils.h"
namespace wtv
{
	VulkanFramebuffer::VulkanFramebuffer(Properties&& params, VulkanDevice* engine) :
		IFramebuffer(std::move(params)),
		m_engine(engine)
	{
		m_attachments.reserve(m_params.layout.GetAttachmentCount());
		bool extentInitialized = false;
		uint32_t width, height, arraySize;
		for (auto colorBuf : m_params.colorBuffers)
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
		if (m_params.depthBuffer.has_value())
		{
			VulkanGPUImage* img = reinterpret_cast<VulkanGPUImage*>(m_params.depthBuffer->image);
			m_attachments.push_back(img->GetImageView(m_params.depthBuffer.value()));
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
		createInfo.renderPass = m_engine->ObtainDummyRenderPass(m_params.layout)->GetNativeHandle();
		ASSERT_VK_SUCCESS(vkCreateFramebuffer(m_engine->GetDevice(), &createInfo, nullptr, &m_framebuffer));
	}

	
	IServiceProvider* VulkanFramebuffer::GetServiceProvider()
	{
		return m_engine->GetServiceProvider();
	}
}