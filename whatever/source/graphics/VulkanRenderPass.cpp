#include "VulkanRenderPass.h"
#include "FormatUtils.h"
#include "VulkanConstantTranslator.h"
#include "VulkanDevice.h"

namespace wtv
{
VulkanRenderPass::VulkanRenderPass(VulkanDevice* device, const RenderPassParams& params) :
	m_device(device),
	m_properties(params)
{
	std::vector<VkAttachmentDescription> attachmentDescs(params.GetAttachmentCount());
	for (int i = 0; i < attachmentDescs.size(); ++i)
	{
		// TODO: image layouts, stencil stuff
		attachmentDescs[i] = {};
		attachmentDescs[i].format = VulkanConstantTranslator::GetVkFormat(params.GetRTInfo(i).format);
		attachmentDescs[i].loadOp = VulkanConstantTranslator::GetVkAttachmentLoadOp(params.GetRTInfo(i).loadOp);
		attachmentDescs[i].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachmentDescs[i].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachmentDescs[i].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachmentDescs[i].initialLayout = VulkanConstantTranslator::GetVkImageLayout(params.GetRTInfo(i).layoutBefore);
		attachmentDescs[i].finalLayout = VulkanConstantTranslator::GetVkImageLayout(params.GetRTInfo(i).layoutAfter);
		attachmentDescs[i].samples = VK_SAMPLE_COUNT_1_BIT;
	}

	std::vector<VkAttachmentReference> attachmentRefs(attachmentDescs.size());
	for (int i = 0; i < attachmentRefs.size(); ++i)
	{
		attachmentRefs[i].attachment = i;
		attachmentRefs[i].layout = IsDepthFormat(params.GetRTInfo(i).format) ? VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
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

	vkCreateRenderPass(m_device->GetDevice(), &rpInfo, nullptr, &m_renderPass);
}

VulkanRenderPass::~VulkanRenderPass()
{
	vkDestroyRenderPass(m_device->GetDevice(), m_renderPass, nullptr);
}

}
