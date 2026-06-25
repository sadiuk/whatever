#include "VulkanDebugNamer.h"
#include "VulkanDevice.h"

namespace wtv
{
	void VulkanDebugNamer::Init(VulkanDevice* device)
	{
		m_device = device;

		m_vkDevice = device->GetDevice();
		auto instance = m_device->GetInstance();
		vkSetDebugUtilsObjectNameEXT = (PFN_vkSetDebugUtilsObjectNameEXT)
			vkGetInstanceProcAddr(instance, "vkSetDebugUtilsObjectNameEXT");
	}
}