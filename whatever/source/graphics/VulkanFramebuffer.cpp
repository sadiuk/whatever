#include "VulkanFramebuffer.h"
#include "VulkanGPUImage.h"
#include "VkMakros.h"
#include "FormatUtils.h"
namespace wtv
{
	VulkanFramebuffer::VulkanFramebuffer(CreateInfo&& params, VulkanDevice* engine) :
		IFramebuffer(std::move(params)),
		m_engine(engine)
	{
		CreateDummyRenderPass();
		m_attachments.reserve(m_params.GetAttachmentCount());
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
		createInfo.renderPass = m_dummyRenderPass;
		ASSERT_VK_SUCCESS(vkCreateFramebuffer(m_engine->GetDevice(), &createInfo, nullptr, &m_framebuffer));
	}

	void VulkanFramebuffer::CreateDummyRenderPass()
	{
		std::vector<VkAttachmentDescription> attachmentDescs(m_params.GetAttachmentCount());
		for (int i = 0; i < attachmentDescs.size(); ++i)
		{
			// TODO: image layouts, stencil stuff
			attachmentDescs[i] = {};
			attachmentDescs[i].format = VulkanConstantTranslator::GetVkFormat(m_params.GetRTInfo(i).format);
			attachmentDescs[i].loadOp = m_params.GetRTInfo(i).clearBeforeWrite ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
			attachmentDescs[i].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			attachmentDescs[i].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			attachmentDescs[i].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			attachmentDescs[i].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			attachmentDescs[i].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			attachmentDescs[i].samples = VK_SAMPLE_COUNT_1_BIT;
		}

		std::vector<VkAttachmentReference> attachmentRefs(attachmentDescs.size());
		for (int i = 0; i < attachmentRefs.size(); ++i)
		{
			attachmentRefs[i].attachment = i;
			attachmentRefs[i].layout = IsDepthFormat(m_params.GetRTInfo(i).format) ? VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		}
		VkSubpassDescription subpassDesc{};
		subpassDesc.colorAttachmentCount = attachmentRefs.size();
		subpassDesc.pColorAttachments = attachmentRefs.data();
		subpassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;


		VkRenderPassCreateInfo rpInfo{};
		rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		rpInfo.pNext = nullptr;
		rpInfo.attachmentCount = (uint32_t)attachmentDescs.size();
		rpInfo.pAttachments = attachmentDescs.data();
		rpInfo.dependencyCount = 0;
		rpInfo.pDependencies = nullptr;
		rpInfo.subpassCount = 1;
		rpInfo.pSubpasses = &subpassDesc;

		ASSERT_VK_SUCCESS(vkCreateRenderPass(m_engine->GetDevice(), &rpInfo, nullptr, &m_dummyRenderPass));
	}

	IServiceProvider* VulkanFramebuffer::GetServiceProvider()
	{
		return m_engine->GetServiceProvider();
	}
}