#pragma once
#include "GraphicsConstants.h"

#include <vector>
#include <string>
#include <map>
namespace wtv
{
	struct IGraphicsPipeline
	{
		struct StageDesc
		{
			ShaderStage stage;
			std::string entryPoint;
			std::string path;
		};
		struct ViewportInfo
		{
			uint32_t x = 0, y = 0;
			uint32_t width, height;
		};
		struct AttachmentBlendState
		{
			bool blendEnable = true;
			BlendFactor srcColorBlendFactor = BlendFactor::One;
			BlendFactor dstColorBlendFactor = BlendFactor::Zero;
			BlendOperation colorBlendOperation = BlendOperation::Add;
			BlendFactor srcAlphaBlendFactor = BlendFactor::One;
			BlendFactor dstAlphaBlendFactor = BlendFactor::Zero;
			BlendOperation alphaBlendOperation = BlendOperation::Add;
		};
		struct BlendStateInfo
		{
			std::vector<AttachmentBlendState> attachmentBlendStates;
		};
		struct StencilOperation
		{
			StencilTestOperation failOperation = StencilTestOperation::Zero;
			StencilTestOperation passOperation = StencilTestOperation::Zero;
			StencilTestOperation depthFailOperation = StencilTestOperation::Zero;
			CompareOperation compareOperation = CompareOperation::Never;
			uint32_t compareMask = 0;
			uint32_t writeMask = 0;
			uint32_t reference = 0;
		};
		struct DepthStencilInfo
		{
			bool depthTestEnable = true;
			bool depthWriteEnable = true;
			CompareOperation depthTestPassResult;
			bool stencilTestEnabled = false;
			StencilOperation backStencilOp;
			StencilOperation frontStencilOp;
		};
		struct RasterInfo
		{
			bool enableDepthClamp = true;
			FrontFace frontFace;
			CullMode cullMode;
			PolygonMode polygonMode;
		};
		struct DescriptorSetLayout
		{
		};
		struct RenderTargetInfo
		{
			ImageFormat format;
			bool clearBeforeWrite = false;
		};
		using VerterBufferLayout = std::vector<VertexAtributeType>;
		using StagesDescription = std::vector<StageDesc>;
		using RenderTargetInfoVec = std::vector<RenderTargetInfo>;
		struct CreateInfo
		{
			VerterBufferLayout vertexBufferLayout;
			StagesDescription stagesDescription;
			PrimitiveTopology vertexTopology;
			ViewportInfo viewportInfo;
			RasterInfo rasterInfo;
			DepthStencilInfo depthStencilInfo;
			BlendStateInfo blendStateInfo;
			RenderTargetInfoVec renderTargetInfoVec;
		};

		IGraphicsPipeline(const CreateInfo& params) : m_params(params) {}
		virtual ~IGraphicsPipeline() {}
	protected:
		CreateInfo m_params;

	};
}