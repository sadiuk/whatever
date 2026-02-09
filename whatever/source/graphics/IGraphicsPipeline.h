#pragma once
#include "GraphicsConstants.h"
#include "GraphicsStructs.h"
#include "IFramebuffer.h"
#include "IDescriptorSet.h"
#include "util/RefPtr.h"

#include <vector>
namespace wtv
{
	struct GraphicsPipelineLayoutCreateInfo
	{
		IDescriptorSetLayout** descriptorSetLayouts;
		uint32_t descriptorSetLayoutCount = 0;
		PushConstantRange* pcRanges;
		uint32_t pushConstantRangeCount = 0;
	};
	struct IGraphicsPipelineLayout : public IReferenceCounted
	{
		virtual ~IGraphicsPipelineLayout() {}
	};
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
	protected:
		CreateInfo m_params;

	};
}