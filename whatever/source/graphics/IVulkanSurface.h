#pragma once
#include "ISurface.h"

#include <vector>

#include "vulkan/vulkan.h"
namespace wtv
{
	struct IVulkanSurface : public ISurface
	{
		virtual void* GetNativeHandle() = 0;
		virtual void GetRequiredExtensions(VkInstance instance, std::vector<const char*>& outExtensions) = 0;
		virtual bool Initialize(VkInstance instance) = 0;
		virtual bool Deinitialize() = 0;
	};
}