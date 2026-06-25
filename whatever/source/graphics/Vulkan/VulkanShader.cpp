#include "VulkanShader.h"
#include "VkMakros.h"
#include "VulkanDevice.h"
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
		ASSERT_VK_SUCCESS(vkCreateShaderModule(params.engine->GetDevice(), &moduleInfo, nullptr, &m_module));
	}

	VulkanShader::~VulkanShader()
	{
		vkDestroyShaderModule(m_params.engine->GetDevice(), m_module, nullptr);
	}

	IServiceProvider* VulkanShader::GetServiceProvider() const
	{
		return m_params.engine->GetServiceProvider();
	}
}