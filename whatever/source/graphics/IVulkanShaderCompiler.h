#pragma once
#include "VulkanShader.h"
#include "GraphicsConstants.h"
#include "IService.h"

#include <memory>
#include <filesystem>


struct IVulkanShaderCompiler : public IService
{
	IMPL_ISERVICE(IVulkanShaderCompiler);

	struct CompilationParams
	{
		VkDevice device;
		std::string entryPoint;
		ShaderStage stage;
		std::filesystem::path sourcePath;
	};
	virtual std::shared_ptr<VulkanShader> CreateShaderFromFile(const CompilationParams& params) = 0;
};