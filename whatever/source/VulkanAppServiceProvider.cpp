#include "VulkanAppServiceProvider.h"

#include "graphics/Vulkan/VulkanShaderCompilerDXC.h"
#include <util/ILogger.h>
#include <util/ConsoleLogger.h>
#include "util/Settings.h"

namespace wtv
{
	VulkanAppServiceProvider::VulkanAppServiceProvider()
	{
		std::shared_ptr<IVulkanShaderCompiler> compiler = std::make_shared<VulkanShaderCompilerDXC>();
		std::shared_ptr<ILogger> logger = std::make_shared<ConsoleLogger>();
		std::shared_ptr<Settings> settings = std::make_shared<Settings>("settings.txt");

		AddService(compiler);
		AddService(logger);
		AddService(settings);
	}
}