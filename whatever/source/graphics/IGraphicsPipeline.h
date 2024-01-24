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
	using VerterBufferLayout = std::vector<VertexAtributeType>;
	using StagesDescription = std::vector<StageDesc>;
	struct CreateInfo
	{
		VerterBufferLayout vertexBufferLayout;
		StagesDescription stagesDescription;
	};

	IGraphicsPipeline(const CreateInfo& params) : m_params(params) {}
	virtual ~IGraphicsPipeline() {}
protected:
	CreateInfo m_params;

};