#include "IEngine.h"
#include "VulkanEngine.h"
#include "VulkanSurfaceSDL.h"
std::shared_ptr<IEngine> IEngine::Create(const CreationParams& params)
{
	ISurface::CreationParams surfaceParams;
	surfaceParams.window = params.window;

	auto surfaceFactory = std::make_unique<VulkanSurfaceSDLFactory>(surfaceParams);
	return std::make_shared<VulkanEngine>(params, surfaceFactory.get());
}
