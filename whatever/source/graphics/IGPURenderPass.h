#pragma once
#include "util/RefPtr.h"
#include "GraphicsConstants.h"
#include "IFramebuffer.h"

#include <vector>


namespace wtv
{
	struct AttachmentInfo
	{
		ImageFormat format;
		AttachmentLoadOp loadOp = AttachmentLoadOp::Load;
		AttachmentStoreOp storeOp = AttachmentStoreOp::Store;
		ImageLayout layoutBefore = ImageLayout::Undefined;
		ImageLayout layoutAfter = ImageLayout::Undefined;
		std::optional<glm::vec4> clearColor;
		bool operator==(const AttachmentInfo&) const = default;
	};
	struct RenderPassParams
	{
		std::vector<AttachmentInfo> colorAttachments;
		std::optional<AttachmentInfo> depthAttachment;
		explicit RenderPassParams(const IFramebuffer::Layout& layout)
		{
			colorAttachments.resize(layout.colorBuffers.size());
			for (int i = 0; i < layout.colorBuffers.size(); i++)
			{
				colorAttachments[i].format = layout.colorBuffers[i];
			}
			if (layout.depthBuffer.has_value())
			{
				depthAttachment = AttachmentInfo();
				depthAttachment->format = layout.depthBuffer.value();
			}
		}
		void SetColorAttachmentInfo(int colorBufferIndex, AttachmentLoadOp loadOp, AttachmentStoreOp storeOp, ImageLayout layoutBefore, ImageLayout layoutAfter, const glm::vec4* clearColor = nullptr)
		{
			assert(colorBufferIndex < colorAttachments.size());
			colorAttachments[colorBufferIndex].loadOp = loadOp;
			colorAttachments[colorBufferIndex].storeOp = storeOp;
			colorAttachments[colorBufferIndex].layoutBefore = layoutBefore;
			colorAttachments[colorBufferIndex].layoutAfter = layoutAfter;
			if(clearColor)
				colorAttachments[colorBufferIndex].clearColor = *clearColor;
		}
		void SetDepthAttachmentInfo(AttachmentLoadOp loadOp, AttachmentStoreOp storeOp, ImageLayout layoutBefore, ImageLayout layoutAfter, float* clearDepth = nullptr, uint8_t* clearStencil = nullptr)
		{
			assert(depthAttachment.has_value());
			depthAttachment->loadOp = loadOp;
			depthAttachment->storeOp = storeOp;
			depthAttachment->layoutBefore = layoutBefore;
			depthAttachment->layoutAfter = layoutAfter;
			if (clearDepth || clearStencil)
				depthAttachment->clearColor = glm::vec4{};
			if(clearDepth)
				depthAttachment->clearColor->x = *clearDepth;
			if (clearStencil)
				depthAttachment->clearColor->y = *clearStencil;
		}
		int GetAttachmentCount() const
		{
			int count = (int)colorAttachments.size();
			if (depthAttachment.has_value())
				count += 1;
			return count;
		}
		const AttachmentInfo& GetRTInfo(int index) const
		{
			assert(index >= 0 && index < GetAttachmentCount());
			if (index < colorAttachments.size())
				return colorAttachments[index];
			else
				return depthAttachment.value();
		}
	};

	struct IGPURenderPass : public IReferenceCounted
	{
		virtual ~IGPURenderPass() {}

		virtual const RenderPassParams& GetProperties() const = 0;
	};
}