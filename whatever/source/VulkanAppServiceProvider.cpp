#include "VulkanAppServiceProvider.h"

#include "graphics/VulkanShaderCompilerDXC.h"
#include <util/ILogger.h>
#include <util/ConsoleLogger.h>

namespace wtv
{
	VulkanAppServiceProvider::VulkanAppServiceProvider()
	{
		std::shared_ptr<IVulkanShaderCompiler> compiler = std::make_shared<VulkanShaderCompilerDXC>();
		std::shared_ptr<ILogger> logger = std::make_shared<ConsoleLogger>();

		AddService(compiler);
		AddService(logger);
	}
}