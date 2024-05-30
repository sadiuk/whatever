#include "VulkanEngine.h"
#include "VkMakros.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanFramebuffer.h"
#include "VulkanConstantTranslator.h"
#include "VulkanSwapchain.h"
#include "VulkanGPUImage.h"
#include "VulkanGPUBuffer.h"
#include "VulkanCommandBuffer.h"
#include "util/RefPtr.h"

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
		m_swapchain = MakeRef<VulkanSwapchain>(this, m_surface.get());
		return true;
	}
	bool VulkanEngine::CreateCommandPools()
	{
		for (int i = 0; i < m_swapchain->GetImageCount(); ++i)
			m_commandPools.push_back(VulkanCommandPool(this));
		return true;
	}       
	bool VulkanEngine::CreateCommandQueues()
	{
		m_graphicsQueue = MakeRef<VulkanQueue>(this, m_queueFamilyIndices.graphicsFamilyIndex.value(), 0);
		m_presentQueue = MakeRef<VulkanPresentQueue>(this, m_queueFamilyIndices.graphicsFamilyIndex.value(), 0);
		return true;
	}

	bool VulkanEngine::Init()
	{
		do
		{
			if (!CreateInstance(m_creationParams.appName))
				break;
			if (!InitSurface())
				break;
			if (!SelectPhysicalDevice())
				break;
			if (!CreateLogicalDevice())
				break;
			if (!CreateAllocator())
				break;
			if (!CreateSwapChain())
				break;
			if (!CreateCommandPools())
				break;
			if (!CreateCommandQueues())
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

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, familyIndex, (VkSurfaceKHR)m_surface->GetNativeHandle(), &presentSupport);
			if (presentSupport)
				m_queueFamilyIndices.presentFamilyIndex = familyIndex;
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
		deviceFeatures.geometryShader = VK_TRUE;
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

	RefPtr<IGraphicsPipeline> VulkanEngine::CreateGraphicsPipeline(const IGraphicsPipeline::CreateInfo& params)
	{
		auto pipeline = MakeRef<VulkanGraphicsPipeline>(this, m_services, params);
		return pipeline;
	}

	RefPtr<ICommandBuffer> VulkanEngine::CreateCommandBuffer()
	{
		return m_commandPools[m_swapchain->GetImageIndex()].CreateCommandBuffer();
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

	uint32_t VulkanEngine::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDeviceMemoryProperties memProps{};
		vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProps);

		for (int i = 0; i < memProps.memoryTypeCount; ++i)
		{
			if ((typeFilter & (1 << i)) && (memProps.memoryTypes[i].propertyFlags & properties == properties))
			{
				return i;
			}
		}
	}

	RefPtr<IGPUImage> VulkanEngine::GetBackbuffer()
	{
		return m_swapchain->GetBackBuffer();
	}

	RefPtr<IFence> VulkanEngine::CreateFence(bool createSignaled)
	{
		return MakeRef<VulkanFence>(this, createSignaled);
	}

	RefPtr<IGPUBuffer> VulkanEngine::CreateBuffer(const IGPUBuffer::CreationParams params)
	{
		auto buffer = MakeRef<VulkanGPUBuffer>(this, params);
		return buffer;
	}

	ImageFormat VulkanEngine::GetSwapchainFormat()
	{
		return VulkanConstantTranslator::GetEngineImageFormat(m_swapchain->GetFormat());
	}

	void VulkanEngine::Submit(ICommandBuffer* cb)
	{
		m_graphicsQueue->Submit(cb);
	}

	void VulkanEngine::Present()
	{
		m_presentQueue->PresentSwapchainImage(m_swapchain.get(), m_graphicsQueue.get());
	}

	void VulkanEngine::BeginFrame()
	{
		m_graphicsQueue->GetFence().Wait();
		m_graphicsQueue->GetFence().Reset();
		m_commandPools[m_swapchain->GetImageIndex()].WaitCommandBuffersAndClear();
		m_swapchain->GetNextImage();
	}


	bool VulkanEngine::Deinit()
	{

		vkDestroyDevice(m_device, nullptr);
		vkDestroyInstance(m_instance, nullptr);

		return true;
	}
}