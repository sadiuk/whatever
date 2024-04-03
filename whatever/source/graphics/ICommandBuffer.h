#pragma once
#include "util/RefPtr.h"
#include "IGraphicsPipeline.h"
namespace wtv
{
	struct ICommandBuffer : public IReferenceCounted
	{
		virtual void Reset() = 0;
		virtual void Begin() = 0;
		virtual void End() = 0;
		virtual void SetViewport(const ViewportInfo& viewport) = 0;
		virtual void SetScissor(const Rect2D& scissor) = 0;
		virtual void BindPipelineAndFramebuffer(IGraphicsPipeline* pipeline, IFramebuffer* framebuffer) = 0;
		virtual void SetClearColorValue(uint32_t colorAttachmentIndex, void* clearColor) = 0;
		virtual void SetClearDepthStencilValue(float depth, uint32_t stencil = 0) = 0;
		virtual void Draw(uint32_t vertexCount, uint32_t firstVertex, uint32_t instanceCount = 0, uint32_t firstInstance = 0) = 0;

		virtual ~ICommandBuffer() = default;
	};
}