#pragma once
#include "vulkan/vulkan.h"
namespace wtv
{
	class VulkanDevice;
	class VulkanDebugNamer
	{
	public:
		VulkanDebugNamer() = default;
		void Init(VulkanDevice* device);
		
		void SetBufferName(VkBuffer buffer, const char* name) const;
	private:
		VulkanDevice* m_device;
		PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT{};
	};
}