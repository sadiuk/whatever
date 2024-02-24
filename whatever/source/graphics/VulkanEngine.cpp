#include "VulkanEngine.h"
#include "VkMakros.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanFramebuffer.h"
#include "VulkanConstantTranslator.h"

#define VMA_IMPLEMENTATION
#include "vma/vk_mem_alloc.h"
#include "SDL.h"

#include <algorithm>

namespace wtv
{
	VulkanEngine::VulkanEngine(const IEngine::CreationParams& params, IServiceProvider* services, ISurfaceFactory* factory) :
		IEngine(services),
		m_creationParams(params)
	{
		m_surface = StaticRefCast<IVulkanSurface>(factory->Create());
		Init();

	}

	VulkanEngine::~VulkanEngine()
	{
		Deinit();
	}

	bool VulkanEngine::CreateSwapChain()
	{
		VkSwapchainCreateInfoKHR swapchainCreateInfo{};


		AvailableSwapchainCapabilities caps = GetAvailableSwapchainCapabilities();
		VkSurfaceFormatKHR surfaceFmt{};

		IImage::CreationParams swapchainImageParams{};

		if (auto surfaceFmtIter = std::find_if(
			caps.availableFormats.begin(), caps.availableFormats.end(),
			[](const VkSurfaceFormatKHR& fmt) { return fmt.format == VK_FORMAT_B8G8R8A8_SRGB; });
			surfaceFmtIter != caps.availableFormats.end())
		{
			surfaceFmt = *surfaceFmtIter;
		}
		else
		{
			assert(false);
			surfaceFmt = caps.availableFormats[0];
		}
		swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCreateInfo.pNext = nullptr;
		swapchainCreateInfo.flags = 0;
		swapchainCreateInfo.surface = (VkSurfaceKHR)m_surface->GetNativeHandle();
		swapchainCreateInfo.minImageCount = 3;
		swapchainCreateInfo.imageFormat = m_swapchainFormat = surfaceFmt.format;
		swapchainCreateInfo.imageColorSpace = surfaceFmt.colorSpace;
		swapchainCreateInfo.imageExtent = caps.surfaceCaps.currentExtent;
		swapchainCreateInfo.imageArrayLayers = 1;
		swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		
		swapchainImageParams.width = swapchainCreateInfo.imageExtent.width;
		swapchainImageParams.height = swapchainCreateInfo.imageExtent.height;
		swapchainImageParams.arraySize = swapchainCreateInfo.imageArrayLayers;
		swapchainImageParams.format = VulkanConstantTranslator::GetEngineImageFormat(swapchainCreateInfo.imageFormat);

		if (m_queueFamilyIndices.graphicsFamilyIndex.value() != m_queueFamilyIndices.computeFamilyIndex.value())
		{
			uint32_t queuefamilyIndices[2] = {
				m_queueFamilyIndices.graphicsFamilyIndex.value(),
				m_queueFamilyIndices.computeFamilyIndex.value()
			};
			swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			swapchainCreateInfo.queueFamilyIndexCount = 2;
			swapchainCreateInfo.pQueueFamilyIndices = queuefamilyIndices;
		}
		else
		{
			uint32_t queueFamilyIndex = m_queueFamilyIndices.graphicsFamilyIndex.value();
			swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			swapchainCreateInfo.queueFamilyIndexCount = 1;
			swapchainCreateInfo.pQueueFamilyIndices = &queueFamilyIndex;
		}
		swapchainCreateInfo.preTransform = caps.surfaceCaps.currentTransform;
		swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainCreateInfo.presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
		swapchainCreateInfo.clipped = VK_TRUE;
		swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

		ASSERT_VK_SUCCESS_ELSE_RET0(vkCreateSwapchainKHR(m_device, &swapchainCreateInfo, nullptr, &m_swapchain));

		return CreateSwapchainImages(swapchainImageParams);

	}
	bool VulkanEngine::CreateSwapchainImages(const IImage::CreationParams& params)
	{
		uint32_t imageCount{};
		ASSERT_VK_SUCCESS_ELSE_RET0(vkGetSwapchainImagesKHR(m_device, m_swapchain, &imageCount, nullptr));
		std::vector<VkImage> rawSwapchainImages;
		rawSwapchainImages.resize(imageCount);
		ASSERT_VK_SUCCESS_ELSE_RET0(vkGetSwapchainImagesKHR(m_device, m_swapchain, &imageCount, rawSwapchainImages.data()));

		m_swapchainImages.resize(imageCount);
		for (int i = 0; i < m_swapchainImages.size(); ++i)
		{
			m_swapchainImages[i] = MakeRef<VulkanGPUImage>(m_device, params, rawSwapchainImages[i]);
		}
		return true;
	}


	bool VulkanEngine::Init()
	{
		do
		{
			if (!CreateInstance(m_creationParams.appName))
				break;
			if (!SelectPhysicalDevice())
				break;
			if (!CreateLogicalDevice())
				break;
			if (!CreateAllocator())
				break;
			if (!InitSurface())
				break;
			if (!CreateSwapChain())
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


		std::vector<const char*> extensions{};
		m_surface->GetRequiredExtensions(m_instance, extensions);
		VkInstanceCreateInfo instanceCreateInfo{};
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.pNext = nullptr;
		instanceCreateInfo.flags = 0;
		instanceCreateInfo.pApplicationInfo = &appInfo;
		instanceCreateInfo.enabledExtensionCount = (uint32_t)extensions.size();
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

		const std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};
		VkPhysicalDeviceFeatures deviceFeatures{};
		VkDeviceCreateInfo deviceInfo = {};
		deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceInfo.pNext = nullptr;
		deviceInfo.queueCreateInfoCount = 1;
		deviceInfo.pQueueCreateInfos = queueInfos;
		deviceInfo.enabledExtensionCount = (uint32_t)deviceExtensions.size();
		deviceInfo.ppEnabledExtensionNames = deviceExtensions.data();
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

	VulkanEngine::AvailableSwapchainCapabilities VulkanEngine::GetAvailableSwapchainCapabilities()
	{
		AvailableSwapchainCapabilities caps{};
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, (VkSurfaceKHR)m_surface->GetNativeHandle(), &caps.surfaceCaps);

		uint32_t formatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, (VkSurfaceKHR)m_surface->GetNativeHandle(), &formatCount, nullptr);
		caps.availableFormats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, (VkSurfaceKHR)m_surface->GetNativeHandle(), &formatCount, caps.availableFormats.data());

		uint32_t modeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, (VkSurfaceKHR)m_surface->GetNativeHandle(), &modeCount, nullptr);
		caps.presentModes.resize(modeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, (VkSurfaceKHR)m_surface->GetNativeHandle(), &modeCount, caps.presentModes.data());
		return caps;
	}

	RefPtr<IGraphicsPipeline> VulkanEngine::CreateGraphicsPipeline(const IGraphicsPipeline::CreateInfo& params)
	{
		auto pipeline = MakeRef<VulkanGraphicsPipeline>(this, m_services, params);
		return pipeline;
	}

	RefPtr<IFramebuffer> VulkanEngine::CreateFramebuffer(const IFramebuffer::CreateInfo& params, VkRenderPass renderpass)
	{
		if (params.colorBuffers.size() > 1)
		{
			for (int i = 1; i < params.colorBuffers.size(); ++i)
			{
				if (!params.colorBuffers[i].HasSameExtent(params.colorBuffers[i - 1])) return nullptr;
				// Log that all image views must have same extent
			}
		}
		auto framebuffer = MakeRef<VulkanFramebuffer>(params, m_device, renderpass);
		return framebuffer;
	}

	std::vector<RefPtr<IGPUImage>> VulkanEngine::GetSwapchainImages()
	{
		std::vector<RefPtr<IGPUImage>> result(m_swapchainImages.size());
		for (int i = 0; i < m_swapchainImages.size(); ++i)
		{
			result[i] = m_swapchainImages[i];
		}
		return result;
	}

	ImageFormat VulkanEngine::GetSwapchainFormat()
	{
		return VulkanConstantTranslator::GetEngineImageFormat(m_swapchainFormat);
	}


	bool VulkanEngine::Deinit()
	{
		vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
		m_surface->Deinitialize(m_instance);

		vkDestroyDevice(m_device, nullptr);
		vkDestroyInstance(m_instance, nullptr);

		return true;
	}
}