#include "IEngine.h"
#include "VulkanEngine.h"
#include "VulkanSurfaceSDL.h"
#include <IServiceProvider.h>
#include "util/RefPtr.h"
namespace wtv
{
	RefPtr<IEngine> IEngine::Create(const CreationParams& params, IServiceProvider* services)
	{
		ISurface::CreationParams surfaceParams;
		surfaceParams.window = params.window;

		auto surfaceFactory = std::make_unique<VulkanSurfaceSDLFactory>(surfaceParams);
		return MakeRef<VulkanEngine>(params, services, surfaceFactory.get());
	}
}