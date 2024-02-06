#pragma once
#include <memory>
#include <vector>
#include <string>

#include "vulkan/vulkan.h"

#include "IVulkanSurface.h"

#include "ui/IWindow.h"
namespace wtv
{
	struct VulkanSurfaceSDLFactory : ISurfaceFactory
	{
		VulkanSurfaceSDLFactory(const ISurface::CreationParams& params) : ISurfaceFactory(params) {}
		std::shared_ptr<ISurface> Create();
	};
	class VulkanSurfaceSDL : public IVulkanSurface
	{
	public:
		VulkanSurfaceSDL(const ISurface::CreationParams& params);
		void* GetNativeHandle() override;
		void GetRequiredExtensions(VkInstance instance, std::vector<const char*>& outExtensions) override;
		bool Initialize(VkInstance instance) override;
		bool Deinitialize(VkInstance instance) override;
	private:
		VkSurfaceKHR m_surface;
		std::shared_ptr<IWindow> m_window;
	};
}