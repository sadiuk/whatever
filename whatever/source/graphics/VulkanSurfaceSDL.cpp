#include "VulkanSurfaceSDL.h"
#include "VkMakros.h"

#include "SDL_vulkan.h"
#include <cassert>
namespace wtv
{
	VulkanSurfaceSDL::VulkanSurfaceSDL(const ISurface::CreationParams& params)
	{
		m_window = params.window;
	}

	void* VulkanSurfaceSDL::GetNativeHandle()
	{
		return m_surface;
	}

	void VulkanSurfaceSDL::GetRequiredExtensions(VkInstance instance, std::vector<const char*>& outExtensions)
	{
		auto sdlWindow = (SDL_Window*)m_window->GetNativeHandle();
		uint32_t extCount = 0;
		SDL_Vulkan_GetInstanceExtensions(sdlWindow, &extCount, nullptr);
		outExtensions.resize(extCount);
		SDL_Vulkan_GetInstanceExtensions(sdlWindow, &extCount, outExtensions.data());
	}

	bool VulkanSurfaceSDL::Initialize(VkInstance instance)
	{
		bool res = SDL_Vulkan_CreateSurface((SDL_Window*)m_window->GetNativeHandle(), instance, &m_surface);
		return res;
	}

	bool VulkanSurfaceSDL::Deinitialize(VkInstance instance)
	{
		vkDestroySurfaceKHR(instance, m_surface, nullptr);
		return true;
	}


	RefPtr<ISurface> VulkanSurfaceSDLFactory::Create()
	{
		return MakeRef<VulkanSurfaceSDL>(m_params);
	}
}