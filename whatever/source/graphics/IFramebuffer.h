#pragma once
#include "GraphicsConstants.h"
#include "IGPUImage.h"
#include "util/RefPtr.h"

#include <vector>
#include <optional>

namespace wtv
{
	struct IFramebuffer : public IReferenceCounted
	{
		struct RenderTargetInfo
		{
			ImageFormat format;
			bool clearBeforeWrite = false;
		};
		struct Layout
		{
			std::vector<RenderTargetInfo> colorBuffers;
			std::vector<RenderTargetInfo> depthBuffers;
		};
		struct CreateInfo
		{
			Layout layout;
			std::vector<IImage::View> colorBuffers;
			std::optional<IImage::View> depthBuffer;

			int GetAttachmentCount() const
			{
				int count = (int)colorBuffers.size();
				if (depthBuffer.has_value())
					count += 1;
				return count;
			}
			RenderTargetInfo& GetRTInfo(int index)
			{
				assert(index >= 0 && index < GetAttachmentCount());
				if (index < colorBuffers.size())
					return layout.colorBuffers[index];
				else
					return layout.depthBuffers[index - (int)colorBuffers.size()];
			}
		};
		IFramebuffer(CreateInfo&& params) : m_params(std::move(params)) {}
	protected:
		CreateInfo m_params;

	};

}
