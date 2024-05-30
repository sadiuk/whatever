#pragma once
#include "GraphicsConstants.h"
#include "GraphicsStructs.h"
#include "IFramebuffer.h"
#include "util/RefPtr.h"

#include <vector>
#include <string>
#include <map>
namespace wtv
{
	struct IGraphicsPipeline : public IReferenceCounted
	{
		using VertexBufferLayout = std::vector<VertexAtributeType>;
		using StagesDescription = std::vector<StageDesc>;

		struct CreateInfo
		{
			VertexBufferLayout vertexBufferLayout;
			StagesDescription stagesDescription;
			PrimitiveTopology vertexTopology;
			ViewportInfo viewportInfo;
			RasterInfo rasterInfo;
			DepthStencilInfo depthStencilInfo;
			BlendStateInfo blendStateInfo;
			IFramebuffer::Layout framebufferLayout;
		};


		IGraphicsPipeline(const CreateInfo& params) : m_params(params) {}
		virtual ~IGraphicsPipeline() {}
		virtual RefPtr<IFramebuffer> CreateFramebuffer(const IFramebuffer::CreateInfo& params) = 0;
	protected:
		CreateInfo m_params;

	};
}