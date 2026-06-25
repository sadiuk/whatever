#pragma once
#include "graphics/IShader.h"
#include "graphics/GraphicsConstants.h"
#include "IServiceProvider.h"
#include "graphics/IGPUResource.h"
#include "vulkan/vulkan.h"

#include <vector>
namespace wtv
{
	class VulkanDevice;
	class VulkanShader : public IShader, public IServiceProviderHolder
	{
	public:
		struct CreationParams
		{
			VulkanDevice* engine;
			uint32_t* spirvCode;
			size_t codeLength;
			ShaderStage stage;
			std::string shaderName;
		};

		VulkanShader(const CreationParams& params);
		~VulkanShader();
		VkShaderModule GetNativeHandle() { return m_module; }

		IServiceProvider* GetServiceProvider() const override;
	private:
		CreationParams m_params;
		VkShaderModule m_module;
	};
}