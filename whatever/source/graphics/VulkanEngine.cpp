#include "VulkanEngine.h"
#include "VkMakros.h"

#define VMA_IMPLEMENTATION
#include "vma/vk_mem_alloc.h"
#include "SDL.h"

#include <algorithm>

VulkanEngine::VulkanEngine(const std::string& appName, ISurfaceFactory* factory)
{
	m_surface = std::static_pointer_cast<IVulkanSurface>(factory->Create());
	Init(appName);

}

VulkanEngine::~VulkanEngine()
{
	Deinit();
}

bool VulkanEngine::Init(const std::string& appName)
{
	do
	{
		if (!CreateInstance(appName))
			break;
		if (!SelectPhysicalDevice())
			break;
		if (!CreateLogicalDevice())
			break;
		if (!CreateAllocator())
			break;
		if (!InitSurface())
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
	appInfo.apiVersion = VULKAN_VERSION;

#if USE_VALIDATION_LAYERS
	std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
	assert(EnsureValidationLayersAvailable(validationLayers));
#else
	std::vector<const char*> validationLayers{};
#endif

	
	std::vector<const char*> extensions {};
	m_surface->GetRequiredExtensions(m_instance, extensions);
	VkInstanceCreateInfo instanceCreateInfo{};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pNext = nullptr;
	instanceCreateInfo.flags = 0;
	instanceCreateInfo.pApplicationInfo = &appInfo;
	instanceCreateInfo.enabledExtensionCount = extensions.size();
	instanceCreateInfo.ppEnabledExtensionNames = extensions.data();
	instanceCreateInfo.enabledLayerCount = (uint32_t)validationLayers.size();
	instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();


	ASSERT_VK_SUCCESS_ELSE_RET0(vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance));
	
	return true;
}

bool VulkanEngine::SelectPhysicalDevice()
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
		if (!m_queueFamilyIndices.graphicsFamilyIndex.has_value() && family.queueFlags | VK_QUEUE_GRAPHICS_BIT)
			m_queueFamilyIndices.graphicsFamilyIndex = familyIndex;
		if (!m_queueFamilyIndices.computeFamilyIndex.has_value() && family.queueFlags | VK_QUEUE_COMPUTE_BIT)
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
	VkDeviceQueueCreateInfo queueInfos[1] = {};
	queueInfos[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueInfos[0].pNext = nullptr;
	queueInfos[0].queueCount = 1;
	queueInfos[0].queueFamilyIndex = m_queueFamilyIndices.graphicsFamilyIndex.value();
	queueInfos[0].pQueuePriorities = &queuePriority;
	assert(m_queueFamilyIndices.graphicsFamilyIndex.value() == m_queueFamilyIndices.computeFamilyIndex.value());

	VkPhysicalDeviceFeatures deviceFeatures{};
	VkDeviceCreateInfo deviceInfo = {};
	deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.pNext = nullptr;
	deviceInfo.queueCreateInfoCount = 1;
	deviceInfo.pQueueCreateInfos = queueInfos;
	deviceInfo.enabledExtensionCount = 0;
	deviceInfo.ppEnabledExtensionNames = nullptr;
	deviceInfo.enabledLayerCount = 0;
	deviceInfo.ppEnabledLayerNames = nullptr;
	deviceInfo.pEnabledFeatures = &deviceFeatures;
	
	ASSERT_VK_SUCCESS_ELSE_RET0(vkCreateDevice(m_physicalDevice, &deviceInfo, nullptr, &m_device));

	return true;
}

bool VulkanEngine::CreateAllocator()
{
	VmaAllocatorCreateInfo allocatorInfo{};
	allocatorInfo.device = m_device;
	allocatorInfo.flags = 0;
	allocatorInfo.vulkanApiVersion = VULKAN_VERSION;
	allocatorInfo.physicalDevice = m_physicalDevice;
	allocatorInfo.instance = m_instance;

	ASSERT_VK_SUCCESS_ELSE_RET0(vmaCreateAllocator(&allocatorInfo, &m_allocator));
	return true;
}

bool VulkanEngine::InitSurface()
{
	return m_surface->Initialize(m_instance);
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

bool VulkanEngine::Deinit()
{
	m_surface->Deinitialize(m_instance);

	vkDestroyDevice(m_device, nullptr);
	vkDestroyInstance(m_instance, nullptr);
	
	return true;
}
