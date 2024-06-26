#include "VulkanCommandBuffer.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanFramebuffer.h"
#include "VulkanEngine.h"
#include "VkMakros.h"
#include "VulkanGPUBuffer.h"

wtv::VulkanCommandBuffer::VulkanCommandBuffer(VulkanEngine* engine, VkCommandPool commandPool) :
	m_device(engine->GetDevice()),
	m_commandPool(commandPool),
	m_queueWaitSemaphore(engine),
	m_queueSignalSemaphore(engine),
	m_queueSignalFence(engine)
{
	VkCommandBufferAllocateInfo bufferAllocInfo{};
	bufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	bufferAllocInfo.pNext = nullptr;
	bufferAllocInfo.commandPool = commandPool;
	bufferAllocInfo.commandBufferCount = 1;
	bufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	ASSERT_VK_SUCCESS(vkAllocateCommandBuffers(m_device, &bufferAllocInfo, &m_commandBuffer));
}

wtv::VulkanCommandBuffer::~VulkanCommandBuffer()
{
	vkFreeCommandBuffers(m_device, m_commandPool, 1, &m_commandBuffer);
}

void wtv::VulkanCommandBuffer::Reset()
{
	vkResetCommandBuffer(m_commandBuffer, 0);
}

void wtv::VulkanCommandBuffer::Begin()
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.pNext = nullptr;
	beginInfo.pInheritanceInfo = nullptr;
	ASSERT_VK_SUCCESS(vkBeginCommandBuffer(m_commandBuffer, &beginInfo));
}

void wtv::VulkanCommandBuffer::End()
{
	if(m_renderpass.has_value())
		vkCmdEndRenderPass(m_commandBuffer);
	ASSERT_VK_SUCCESS(vkEndCommandBuffer(m_commandBuffer));
}

void wtv::VulkanCommandBuffer::SetViewport(const ViewportInfo& viewport)
{
	VkViewport vkViewport{};
	vkViewport.x = (float)viewport.x;
	vkViewport.y = (float)viewport.y;
	vkViewport.width = (float)viewport.width;
	vkViewport.height = (float)viewport.height;
	vkViewport.minDepth = 1;
	vkViewport.maxDepth = 0;
	
	vkCmdSetViewport(m_commandBuffer, 0, 1, &vkViewport);
}

void wtv::VulkanCommandBuffer::SetScissor(const Rect2D& scissor)
{
	VkRect2D vkScissor;
	vkScissor.offset = { scissor.x, scissor.y };
	vkScissor.extent = { scissor.width, scissor.height };
	vkCmdSetScissor(m_commandBuffer, 0, 1, &vkScissor);
	m_scissor = vkScissor;
}

void wtv::VulkanCommandBuffer::BindPipelineAndFramebuffer(IGraphicsPipeline* pipeline, IFramebuffer* framebuffer)
{
	auto vkPipeline = static_cast<VulkanGraphicsPipeline*>(pipeline);
	m_renderpass = vkPipeline->GetRenderPass();
	auto vkFramebuffer = static_cast<VulkanFramebuffer*>(framebuffer);
	auto allAttachments = vkFramebuffer->GetAttachments();
	std::vector<VkClearValue> clearValues(allAttachments.size());
	uint32_t colorAttachmentCount = m_clearDepthStencil.has_value() ? allAttachments.size() - 1 : allAttachments.size();
	for (uint32_t i = 0; i < allAttachments.size(); ++i)
	{
		if (i < colorAttachmentCount)
		{
			if (auto cc = std::find_if(m_clearColorValues.begin(), m_clearColorValues.end(), [i](const auto& at) { return at.first == i; }); cc != m_clearColorValues.end())
			{
				std::memcpy(&clearValues[i], cc->second.data(), CLEAR_COLOR_SIZE);
			}
		}
		else
		{
			if (m_clearDepthStencil.has_value())
			{
				std::memcpy(&clearValues[i], &m_clearDepthStencil.value(), sizeof(m_clearDepthStencil.value()));
			}
		}

	}
	VkRenderPassBeginInfo rpInfo{};
	rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	rpInfo.pNext = nullptr;
	rpInfo.renderPass = m_renderpass.value();
	rpInfo.framebuffer = vkFramebuffer->GetNativeHandle();
	rpInfo.clearValueCount = (uint32_t)clearValues.size();
	rpInfo.pClearValues = clearValues.data();
	assert(m_scissor.has_value());
	rpInfo.renderArea = m_scissor.value();
	vkCmdBindPipeline(m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipeline->GetPipeline());
	vkCmdBeginRenderPass(m_commandBuffer, &rpInfo, VK_SUBPASS_CONTENTS_INLINE);

}

void wtv::VulkanCommandBuffer::BindVertexBuffers(IGPUBuffer** buffers, uint32_t count, size_t* offsets)
{
	std::vector<VkBuffer> rawBuffers(count);
	for (int i = 0; i < rawBuffers.size(); ++i)
		rawBuffers[i] = static_cast<VulkanGPUBuffer*>(buffers[i])->GetNativeHandle();
	vkCmdBindVertexBuffers(m_commandBuffer, 0, count, rawBuffers.data(), offsets);
}

void wtv::VulkanCommandBuffer::SetClearColorValue(uint32_t colorAttachmentIndex, void* color)
{
	std::array<std::byte, CLEAR_COLOR_SIZE> clearColor;
	std::memcpy(clearColor.data(), color, CLEAR_COLOR_SIZE);
	m_clearColorValues.emplace_back(std::make_pair(colorAttachmentIndex, clearColor));
}

void wtv::VulkanCommandBuffer::SetClearDepthStencilValue(float depth, uint32_t stencil)
{
	m_clearDepthStencil = std::make_pair(depth, stencil);
}

void wtv::VulkanCommandBuffer::Draw(uint32_t vertexCount, uint32_t firstVertex, uint32_t instanceCount, uint32_t firstInstance)
{
	vkCmdDraw(m_commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}

void wtv::VulkanCommandBuffer::UpdateBuffer(IGPUBuffer* buffer, size_t offset, size_t size, const void* data)
{
	VulkanGPUBuffer* buf = static_cast<VulkanGPUBuffer*>(buffer);
	vkCmdUpdateBuffer(m_commandBuffer, buf->GetNativeHandle(), offset, size, data);
}

wtv::IServiceProvider* wtv::VulkanCommandBuffer::GetServiceProvider()
{
	return m_engine->GetServiceProvider();
}

