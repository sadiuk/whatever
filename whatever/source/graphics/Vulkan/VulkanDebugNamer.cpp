#include "VulkanDebugNamer.h"
#include "VulkanDevice.h"

namespace wtv
{
	void VulkanDebugNamer::Init(VulkanDevice* device)
	{
		m_device = device;
		auto instance = m_device->GetInstance();
		vkSetDebugUtilsObjectNameEXT = (PFN_vkSetDebugUtilsObjectNameEXT)
			vkGetInstanceProcAddr(instance, "vkSetDebugUtilsObjectNameEXT");
	}

	void VulkanDebugNamer::SetBufferName(VkBuffer buffer, const char* name) const
	{
		VkDebugUtilsObjectNameInfoEXT nameInfo{};
		nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
		nameInfo.objectType = VK_OBJECT_TYPE_BUFFER;
		nameInfo.objectHandle = (uint64_t)buffer;
		nameInfo.pObjectName = name;

		if (vkSetDebugUtilsObjectNameEXT != nullptr) {
			vkSetDebugUtilsObjectNameEXT(m_device->GetDevice(), &nameInfo);
		}
	}
}