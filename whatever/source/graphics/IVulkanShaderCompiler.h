#pragma once
#include "VulkanShader.h"
#include "GraphicsConstants.h"
#include "IService.h"
#include <util/RefPtr.h>
#include "VulkanDevice.h"

#include <memory>
#include <filesystem>

namespace wtv
{
	struct IVulkanShaderCompiler : public IService
	{
		IMPL_ISERVICE(IVulkanShaderCompiler);

		struct CompilationParams
		{
			VulkanDevice* engine;
			std::string entryPoint;
			ShaderStage stage;
			std::filesystem::path sourcePath;
		};
		virtual RefPtr<VulkanShader> CreateShaderFromFile(const CompilationParams& params) = 0;
	};
}