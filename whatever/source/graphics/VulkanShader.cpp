#include "VulkanShader.h"
#include "vulkan/vulkan.h"
namespace wtv
{
	VulkanShader::VulkanShader(const CreationParams& params) : m_params(params)
	{
		VkShaderModuleCreateInfo moduleInfo{};
		moduleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		moduleInfo.pNext = nullptr;
		moduleInfo.codeSize = m_params.codeLength;
		moduleInfo.pCode = m_params.spirvCode;
		vkCreateShaderModule(params.device, &moduleInfo, nullptr, &m_module);
	}
}