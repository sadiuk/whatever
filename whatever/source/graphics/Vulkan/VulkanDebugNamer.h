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
		
		template<typename T>
		void SetResourceName(T resource, VkObjectType type, const char* name) const
		{
			if (vkSetDebugUtilsObjectNameEXT != nullptr) {
				VkDebugUtilsObjectNameInfoEXT nameInfo{};
				nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
				nameInfo.objectType = type;
				nameInfo.objectHandle = (uint64_t)resource;
				nameInfo.pObjectName = name;

				vkSetDebugUtilsObjectNameEXT(m_vkDevice, &nameInfo);
			}
		}
	private:
		VulkanDevice* m_device;
		VkDevice m_vkDevice;
		PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT{};
	};
}