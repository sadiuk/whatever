#pragma once
#include "util/RefPtr.h"
#include "IGraphicsPipeline.h"
#include "IGPURenderPass.h"
#include "IGPUBuffer.h"
#include "IDescriptorSet.h"
namespace wtv
{
	struct ICommandBuffer : public IReferenceCounted
	{
		virtual void Reset() = 0;
		virtual void Begin() = 0;
		virtual void End() = 0;
		virtual void BeginRenderPass(IGPURenderPass* rp, IFramebuffer* framebuffer) = 0;
		virtual void EndRenderPass() = 0;
		virtual void SetViewport(const ViewportInfo& viewport) = 0;
		virtual void SetScissor(const Rect2D& scissor) = 0;
		virtual void SetPipelineLayout(const IGraphicsPipelineLayout* layout) = 0;
		virtual void BindPipeline(IGraphicsPipeline* pipeline) = 0;
		virtual void BindVertexBuffers(IGPUBuffer** buffers, uint32_t count, size_t* offsets) = 0;
		virtual void BindIndexBuffer(IGPUBuffer* buffer, size_t offset, IndexType indexType) = 0;
		virtual void SetClearColorValue(uint32_t colorAttachmentIndex, void* clearColor) = 0;
		virtual void SetClearDepthStencilValue(float depth, uint32_t stencil = 0) = 0;
		virtual void Draw(uint32_t vertexCount, uint32_t firstVertex, uint32_t instanceCount = 1, uint32_t firstInstance = 0) = 0;
		virtual void DrawIndexed(uint32_t vertexOffset, uint32_t firstIndex, uint32_t indexCount, uint32_t instanceCount = 1, uint32_t firstInstance = 0) = 0;
		virtual void UpdateBuffer(IGPUBuffer* buffer, size_t offset, size_t size, const void* data) = 0;
		virtual void BindDescriptorSet(uint32_t setIndex, IDescriptorSet* set) = 0;
		virtual void CopyBuffer(IGPUBuffer* src, uint64_t srcOffset, IGPUBuffer* dst, uint64_t dstOffset, uint64_t size) = 0;
		virtual ~ICommandBuffer() = default;
	};
}