#pragma once
#include "IShader.h"
#include "GraphicsConstants.h"
#include "IServiceProvider.h"

#include "vulkan/vulkan.h"

#include <vector>
namespace wtv
{
	class VulkanEngine;
	class VulkanShader : public IShader, public IServiceProviderHolder
	{
	public:
		struct CreationParams
		{
			VulkanEngine* engine;
			uint32_t* spirvCode;
			size_t codeLength;
			ShaderStage stage;
			std::string shaderName;
		};

		VulkanShader(const CreationParams& params);

		VkShaderModule GetNativeHandle() { return m_module; }

		IServiceProvider* GetServiceProvider() override;
	private:
		CreationParams m_params;
		VkShaderModule m_module;
	};
}