#include "VulkanCommandBuffer.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanFramebuffer.h"
#include "VulkanDevice.h"
#include "VkMakros.h"
#include "VulkanGPUBuffer.h"
#include "VulkanGPUImage.h"
#include "VulkanDescriptorSet.h"

wtv::VulkanCommandBuffer::VulkanCommandBuffer(VulkanDevice* engine, VkCommandPool commandPool) :
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

void wtv::VulkanCommandBuffer::BeginRenderPass(IGPURenderPass* rp, IFramebuffer* fb)
{
	VulkanRenderPass* vulkanRP = static_cast<VulkanRenderPass*>(rp);
	VulkanFramebuffer* vulkanFB = static_cast<VulkanFramebuffer*>(fb);
	const RenderPassParams& rpParams = vulkanRP->GetProperties();
	std::vector<VkClearValue> clearValues;
	for (int i = 0; i < rpParams.colorAttachments.size(); i++)
	{
		const auto& entry = rpParams.colorAttachments[i];
		if (entry.clearColor.has_value())
		{
			glm::vec4 value = entry.clearColor.value();
			clearValues.push_back(VkClearValue(VkClearColorValue{ value.x, value.y, value.z, value.w }));
		}
		else
			clearValues.push_back(VkClearValue(VkClearColorValue{ 0.0f, 0.0f, 0.0f, 0.0f }));
	}
	if (rpParams.depthAttachment.has_value() && rpParams.depthAttachment->clearColor.has_value())
	{
		const auto& entry = rpParams.depthAttachment.value();
		glm::vec4 value = entry.clearColor.value();
		clearValues.push_back(VkClearValue{ .depthStencil = VkClearDepthStencilValue{ value.x, (uint32_t)value.y } });
	}
	else
	{
		clearValues.push_back(VkClearValue{ .depthStencil = VkClearDepthStencilValue{ 0.f, 0u } });
	}

	VkRenderPassBeginInfo rpInfo{};
	rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	rpInfo.renderPass = vulkanRP->GetNativeHandle();
	rpInfo.framebuffer = static_cast<VulkanFramebuffer*>(fb)->GetNativeHandle();
	rpInfo.clearValueCount = clearValues.size();
	rpInfo.pClearValues = clearValues.data();	
	
	assert(m_scissor.has_value());
	rpInfo.renderArea = m_scissor.value();
	vkCmdBeginRenderPass(m_commandBuffer, &rpInfo, VK_SUBPASS_CONTENTS_INLINE);

	const auto properties = vulkanFB->GetProperties();
	for (const auto& imageView : properties.colorBuffers)
	{
		VulkanGPUImage* image = static_cast<VulkanGPUImage*>(imageView.image);
		m_resourcesInUse.push_back(image);
	}
	if (properties.depthBuffer.has_value())
	{
		VulkanGPUImage* image = static_cast<VulkanGPUImage*>(properties.depthBuffer.value().image);
		m_resourcesInUse.push_back(image);
	}
}

void wtv::VulkanCommandBuffer::EndRenderPass()
{
	vkCmdEndRenderPass(m_commandBuffer);
}

void wtv::VulkanCommandBuffer::SetViewport(const ViewportInfo& viewport)
{
	VkViewport vkViewport{};
	vkViewport.x = (float)viewport.x;
	vkViewport.y = viewport.height - (float)viewport.y;
	vkViewport.width = (float)viewport.width;
	vkViewport.height = -(float)viewport.height;
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

void wtv::VulkanCommandBuffer::SetPipelineLayout(const IGraphicsPipelineLayout* layout)
{
	m_pipelineLayout = static_cast<const VulkanGraphicsPipelineLayout*>(layout);
}

void wtv::VulkanCommandBuffer::BindPipeline(IGraphicsPipeline* pipeline)
{
	auto vkPipeline = static_cast<VulkanGraphicsPipeline*>(pipeline);
	vkCmdBindPipeline(m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipeline->GetPipeline());

}

void wtv::VulkanCommandBuffer::BindVertexBuffers(IGPUBuffer** buffers, uint32_t count, size_t* offsets)
{
	std::vector<VkBuffer> rawBuffers(count);
	for (int i = 0; i < rawBuffers.size(); ++i)
	{
		auto vulkanBuffer = static_cast<VulkanGPUBuffer*>(buffers[i]);
		rawBuffers[i] = vulkanBuffer->GetNativeHandle();
		m_resourcesInUse.push_back(vulkanBuffer);
	}
	vkCmdBindVertexBuffers(m_commandBuffer, 0, count, rawBuffers.data(), offsets);


}

void wtv::VulkanCommandBuffer::BindIndexBuffer(IGPUBuffer* buffer, size_t offset, IndexType indexType)
{
	auto vulkanBuffer = static_cast<VulkanGPUBuffer*>(buffer);
	vkCmdBindIndexBuffer(m_commandBuffer, vulkanBuffer->GetNativeHandle(), offset, VulkanConstantTranslator::GetVkIndexType(indexType));
	m_resourcesInUse.push_back(vulkanBuffer);
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

void wtv::VulkanCommandBuffer::DrawIndexed(uint32_t vertexOffset, uint32_t firstIndex, uint32_t indexCount, uint32_t instanceCount, uint32_t firstInstance)
{
	vkCmdDrawIndexed(m_commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void wtv::VulkanCommandBuffer::UpdateBuffer(IGPUBuffer* buffer, size_t offset, size_t size, const void* data)
{
	VulkanGPUBuffer* buf = static_cast<VulkanGPUBuffer*>(buffer);
	vkCmdUpdateBuffer(m_commandBuffer, buf->GetNativeHandle(), offset, size, data);
	m_resourcesInUse.push_back(buf);
}

void wtv::VulkanCommandBuffer::BindDescriptorSet(uint32_t setIndex, IDescriptorSet* set)
{
	ASSERT_TRUE(m_pipelineLayout != nullptr);
	auto vulkanDS = static_cast<VulkanDescriptorSet*>(set);
	auto ds = vulkanDS->GetNativeHandle();
	vkCmdBindDescriptorSets(m_commandBuffer,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		m_pipelineLayout->GetNativeHandle(),
		setIndex, 1, &ds, 0, nullptr);

	for (const auto& resource : vulkanDS->GetBoundResources())
	{
		if (resource.second->GetResourceType() == GPUResourceType::Buffer)
		{
			auto buffer = static_cast<VulkanGPUBuffer*>(resource.second);
			m_resourcesInUse.push_back(buffer);
		}
		else if (resource.second->GetResourceType() == GPUResourceType::Image)
		{
			auto image = static_cast<VulkanGPUImage*>(resource.second);
			m_resourcesInUse.push_back(image);
		}
	}
}

void wtv::VulkanCommandBuffer::CopyBuffer(IGPUBuffer* src, uint64_t srcOffset, IGPUBuffer* dst, uint64_t dstOffset, uint64_t size)
{
	VkBuffer srcBuf = static_cast<VulkanGPUBuffer*>(src)->GetNativeHandle();
	VkBuffer dstBuf = static_cast<VulkanGPUBuffer*>(dst)->GetNativeHandle();
	VkBufferCopy copyRegion{};
	copyRegion.srcOffset = srcOffset;
	copyRegion.dstOffset = dstOffset;
	copyRegion.size = size;
	vkCmdCopyBuffer(m_commandBuffer, srcBuf, dstBuf, 1, &copyRegion);
}

void wtv::VulkanCommandBuffer::CopyBufferToImage(IGPUBuffer* src, uint64_t srcOffset, IGPUImage* dst, uint32_t dstSubresource, ImageAspectFlags aspectFlags)
{
	VulkanGPUBuffer* buffer = static_cast<VulkanGPUBuffer*>(src);
	VulkanGPUImage* image = static_cast<VulkanGPUImage*>(dst);
	VkBufferImageCopy region{};
	region.bufferOffset = srcOffset;
	region.bufferRowLength = dst->GetProperties().width;
	region.bufferImageHeight = dst->GetProperties().height;
	VkImageSubresourceLayers subresource{};
	subresource.aspectMask = VulkanConstantTranslator::GetVkImageAspectFlagBits(aspectFlags);
	subresource.baseArrayLayer = dstSubresource / dst->GetProperties().arraySize;
	subresource.mipLevel = dstSubresource % dst->GetProperties().mipLevels;
	subresource.layerCount = dst->GetProperties().arraySize;
	region.imageSubresource = subresource;
	region.imageOffset = {0, 0, 0};
	region.imageExtent = {dst->GetProperties().width, dst->GetProperties().height, 1};

	vkCmdCopyBufferToImage(m_commandBuffer, buffer->GetNativeHandle(), image->GetNativeHandle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
}

void wtv::VulkanCommandBuffer::PipelineBarrier(const ImageBarrier& barrier)
{
	VkDependencyInfo info{};
	info.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;

	info.bufferMemoryBarrierCount = 0;
	info.memoryBarrierCount = 0;
	info.imageMemoryBarrierCount = 1;
	VkImageMemoryBarrier2 imageBarrier{};
	imageBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
	imageBarrier.srcStageMask = VulkanConstantTranslator::GetVkPipelineStageFlags2(barrier.srcStageMask);
	imageBarrier.srcAccessMask = VulkanConstantTranslator::GetVkAccessFlags2(barrier.srcAccessMask);
	imageBarrier.dstStageMask = VulkanConstantTranslator::GetVkPipelineStageFlags2(barrier.dstStageMask);
	imageBarrier.dstAccessMask = VulkanConstantTranslator::GetVkAccessFlags2(barrier.dstAccessMask);
	VulkanGPUImage* vkImage = static_cast<VulkanGPUImage*>(barrier.imageView.image);
	imageBarrier.image = vkImage->GetNativeHandle();
	imageBarrier.oldLayout = VulkanConstantTranslator::GetVkImageLayout(barrier.layoutBefore);
	imageBarrier.newLayout = VulkanConstantTranslator::GetVkImageLayout(barrier.layoutAfter);
	if(barrier.srcQueue)
		imageBarrier.srcQueueFamilyIndex = static_cast<VulkanQueue*>(barrier.srcQueue)->GetQueueFamilyIndex();
	else
		imageBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	if(barrier.dstQueue)
		imageBarrier.dstQueueFamilyIndex = static_cast<VulkanQueue*>(barrier.dstQueue)->GetQueueFamilyIndex();
	else
		imageBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	imageBarrier.subresourceRange.aspectMask = VkImageAspectFlagBits(barrier.imageView.aspectFlags);
	imageBarrier.subresourceRange.baseArrayLayer = barrier.imageView.baseArrayLayer;
	imageBarrier.subresourceRange.layerCount = barrier.imageView.arrayLayerCount;
	imageBarrier.subresourceRange.baseMipLevel = barrier.imageView.baseMip;
	imageBarrier.subresourceRange.levelCount = barrier.imageView.mipCount;
	info.pImageMemoryBarriers = &imageBarrier;

	vkCmdPipelineBarrier2(m_commandBuffer, &info);
}

void wtv::VulkanCommandBuffer::PushConstants(const void* data, uint32_t size, ShaderStageFlags stageFlags)
{
	vkCmdPushConstants(m_commandBuffer, m_pipelineLayout->GetNativeHandle(), VulkanConstantTranslator::GetVkShaderStageFlags(stageFlags), 0, size, data);
}

wtv::IServiceProvider* wtv::VulkanCommandBuffer::GetServiceProvider() const
{
	return m_engine->GetServiceProvider();
}

