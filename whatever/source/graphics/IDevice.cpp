#include "IDevice.h"
#include "Vulkan/VulkanDevice.h"
#include "Vulkan/VulkanSurfaceSDL.h"
#include <IServiceProvider.h>
#include "util/RefPtr.h"
namespace wtv
{
	RefPtr<IDevice> IDevice::Create(const CreationParams& params, IServiceProvider* services)
	{
		ISurface::CreationParams surfaceParams;
		surfaceParams.window = params.window;

		auto surfaceFactory = std::make_unique<VulkanSurfaceSDLFactory>(surfaceParams);
		return MakeRef<VulkanDevice>(params, services, surfaceFactory.get());
	}
}
