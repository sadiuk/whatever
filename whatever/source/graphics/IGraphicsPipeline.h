#pragma once
#include "GraphicsConstants.h"
#include "GraphicsStructs.h"
#include "IFramebuffer.h"
#include "IGPURenderPass.h"
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
			std::vector<DescriptorSetLayout> descriptorSetLayouts;
		};


		IGraphicsPipeline(const CreateInfo& params) : m_params(params) {}
		virtual ~IGraphicsPipeline() {}
	protected:
		CreateInfo m_params;

	};
}