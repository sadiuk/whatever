#include "IEngine.h"
#include "VulkanEngine.h"

std::shared_ptr<IEngine> IEngine::Create(GRAPHICS_API api, const std::string& appName)
{
	return std::make_shared<VulkanEngine>(appName);
}
