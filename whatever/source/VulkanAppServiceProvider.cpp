#include "VulkanAppServiceProvider.h"

#include "graphics/VulkanShaderCompilerDXC.h"

namespace wtv
{
	VulkanAppServiceProvider::VulkanAppServiceProvider()
	{
		std::shared_ptr<IVulkanShaderCompiler> compiler = std::make_shared<VulkanShaderCompilerDXC>();
		AddService(compiler);
	}
}