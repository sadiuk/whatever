#pragma once
#include "GraphicsConstants.h"

#include <vector>
#include <string>
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
	struct RasterInfo
	{
		bool enableDepthClamp = true;
		FrontFace frontFace;
		CullMode cullMode;
		PolygonMode polygonMode;
	};
	using VerterBufferLayout = std::vector<VertexAtributeType>;
	using StagesDescription = std::vector<StageDesc>;
	struct CreateInfo
	{
		VerterBufferLayout vertexBufferLayout;
		StagesDescription stagesDescription;
		PrimitiveTopology vertexTopology;
		ViewportInfo viewportInfo;
		RasterInfo rasterInfo;

	};

	IGraphicsPipeline(const CreateInfo& params) : m_params(params) {}
	virtual ~IGraphicsPipeline() {}
protected:
	CreateInfo m_params;

};