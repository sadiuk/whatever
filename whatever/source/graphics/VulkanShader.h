#pragma once
#include "IShader.h"
#include "GraphicsConstants.h"

#include "vulkan/vulkan.h"

#include <vector>
namespace wtv
{
	class VulkanShader : public IShader
	{
	public:
		struct CreationParams
		{
			VkDevice device;
			uint32_t* spirvCode;
			size_t codeLength;
			ShaderStage stage;
			std::string shaderName;
		};

		VulkanShader(const CreationParams& params);

		VkShaderModule GetNativeHandle() { return m_module; }
	private:
		CreationParams m_params;
		VkShaderModule m_module;
	};
}