#include "VulkanEngine.h"
#include "vma/vk_mem_alloc.h"
#include <algorithm>
#include "VkMakros.h"
VulkanEngine::VulkanEngine(const std::string& appName)
{
	Init(appName);
}

bool VulkanEngine::Init(const std::string& appName)
{
	do
	{
		if (!CreateInstance(appName))
			break;

			return true;
	} while (false);
	return false;
}

bool VulkanEngine::CreateInstance(const std::string& appName)
{
	uint32_t appVersion = 0;

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext = nullptr;
	appInfo.pApplicationName = appName.c_str();
	appInfo.applicationVersion = appVersion;
	appInfo.pEngineName = "whatever";
	appInfo.engineVersion = ENGINE_VERSION;
	appInfo.apiVersion = VK_API_VERSION_1_2;

#if USE_VALIDATION_LAYERS
	std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
	assert(EnsureValidationLayersAvailable(validationLayers));
#else
	std::vector<const char*> validationLayers{};
#endif

	//std::vector<const char*> extensions = { "VK_KHR_win32_surface" };
	VkInstanceCreateInfo instanceCreateInfo{};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pNext = nullptr;
	instanceCreateInfo.flags = 0;
	instanceCreateInfo.pApplicationInfo = &appInfo;
	instanceCreateInfo.enabledExtensionCount = 0;
	instanceCreateInfo.ppEnabledExtensionNames = nullptr;
	instanceCreateInfo.enabledLayerCount = (uint32_t)validationLayers.size();
	instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();


	ASSERT_VK_SUCCESS_ELSE_RET0(vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance));
	
	return true;
}

bool VulkanEngine::CreatePhysicalDevice()
{
	uint32_t deviceCount;
	vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

	m_physicalDevice = devices[0];
	for (const auto& device : devices)
	{
		VkPhysicalDeviceProperties properties = {};
		vkGetPhysicalDeviceProperties(device, &properties);
		//TODO: a nicer way to select physical device;
		if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			m_physicalDevice = device;
		}
	}
	uint32_t queueFamilyPropsCount;
	vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyPropsCount, nullptr);
	std::vector<VkQueueFamilyProperties> queuefamilyProps(queueFamilyPropsCount);
	vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyPropsCount, queuefamilyProps.data());

	uint32_t familyIndex = 0;
	for (const auto& family : queuefamilyProps)
	{
		if (m_queueFamilyIndices.graphicsFamilyIndex.has_value() && family.queueFlags | VK_QUEUE_GRAPHICS_BIT)
			m_queueFamilyIndices.graphicsFamilyIndex = familyIndex;
		if (m_queueFamilyIndices.computeFamilyIndex.has_value() && family.queueFlags | VK_QUEUE_COMPUTE_BIT)
			m_queueFamilyIndices.computeFamilyIndex = familyIndex;

		familyIndex++;
	}

	assert(m_queueFamilyIndices.IsValid());
	if (!m_queueFamilyIndices.IsValid()) return false;

	return true;
}

bool VulkanEngine::CreateLogicalDevice()
{
	float queuePriority = 1.;
	VkDeviceQueueCreateInfo queueInfos[2] = {};
	queueInfos[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueInfos[0].pNext = nullptr;
	queueInfos[0].queueCount = 1;
	queueInfos[0].queueFamilyIndex = m_queueFamilyIndices.graphicsFamilyIndex.value();
	queueInfos[0].pQueuePriorities = &queuePriority;

	VkDeviceCreateInfo deviceInfo = {};
	deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.pNext = nullptr;
	deviceInfo.

	return false;
}

bool VulkanEngine::EnsureValidationLayersAvailable(std::vector<const char*> requestedLayers)
{
	uint32_t layersCount = 0;
	vkEnumerateInstanceLayerProperties(&layersCount, nullptr);
	std::vector<VkLayerProperties> availableLayers(layersCount);
	vkEnumerateInstanceLayerProperties(&layersCount, availableLayers.data());

	bool hasAll = true;
	for (auto& ext : requestedLayers)
	{
		if (availableLayers.end() == std::find_if(availableLayers.begin(), availableLayers.end(),
			[&](const VkLayerProperties& l) { return std::string(l.layerName) == ext; }))
		{
			// Log error
			hasAll = false;
		}
	}


	return hasAll;


}
