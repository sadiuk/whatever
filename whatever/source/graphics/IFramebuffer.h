#pragma once
#include "GraphicsConstants.h"
#include "IGPUImage.h"
#include "util/RefPtr.h"

#include <vector>
#include <unordered_map>

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
			std::vector<IImage::View> depthBuffers;
		};
		IFramebuffer(const CreateInfo& params) : m_params(params) {}
	private:
		CreateInfo m_params;

	};

}
