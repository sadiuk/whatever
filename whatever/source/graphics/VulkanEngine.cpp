#include "VulkanEngine.h"
#include "VkMakros.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanFramebuffer.h"
#include "VulkanConstantTranslator.h"
#include "VulkanSwapchain.h"
#include "VulkanGPUImage.h"
#include "VulkanGPUBuffer.h"
#include "VulkanCommandBuffer.h"
#include "VulkanRenderPass.h"
#include "util/RefPtr.h"

#define VMA_IMPLEMENTATION
#include "vma/vk_mem_alloc.h"
#include "SDL.h"

#include <algorithm>

namespace wtv
{
	VulkanDevice::VulkanDevice(const IDevice::CreationParams& params, IServiceProvider* services, ISurfaceFactory* factory) :
		IDevice(services),
		m_creationParams(params)
	{
		m_surface = StaticRefCast<IVulkanSurface>(factory->Create());
		Init();

	}

	VulkanDevice::~VulkanDevice()
	{
		Deinit();
	}

	bool VulkanDevice::CreateSwapChain()
	{
		m_swapchain = MakeRef<VulkanSwapchain>(this, m_surface.get());
		return true;
	}
	bool VulkanDevice::CreateCommandPools()
	{
		for (int i = 0; i < m_swapchain->GetImageCount(); ++i)
			m_commandPools.push_back(VulkanCommandPool(this));
		return true;
	}       
	bool VulkanDevice::CreateCommandQueues()
	{
		m_graphicsQueue = MakeRef<VulkanQueue>(this, m_queueFamilyIndices.graphicsFamilyIndex.value(), 0);
		m_presentQueue = MakeRef<VulkanPresentQueue>(this, m_queueFamilyIndices.graphicsFamilyIndex.value(), 0);
		return true;
	}

	bool VulkanDevice::Init()
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

	bool VulkanDevice::CreateInstance(const std::string& appName)
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

	bool VulkanDevice::SelectPhysicalDevice()
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

	bool VulkanDevice::CreateLogicalDevice()
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

	bool VulkanDevice::CreateAllocator()
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

	bool VulkanDevice::InitSurface()
	{
		return m_surface->Initialize(m_instance);
	}

	bool VulkanDevice::EnsureValidationLayersAvailable(std::vector<const char*> requestedLayers)
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

	RefPtr<IGraphicsPipeline> VulkanDevice::CreateGraphicsPipeline(const IGraphicsPipeline::CreateInfo& params)
	{
		auto pipeline = MakeRef<VulkanGraphicsPipeline>(this, m_services, params);
		return pipeline;
	}

	RefPtr<ICommandBuffer> VulkanDevice::CreateCommandBuffer()
	{
		return m_commandPools[m_swapchain->GetImageIndex()].CreateCommandBuffer();
	}

	RefPtr<IFramebuffer> VulkanDevice::CreateFramebuffer(IFramebuffer::Properties&& params)
	{
		if (params.colorBuffers.size() > 1)
		{
			for (int i = 1; i < params.colorBuffers.size(); ++i)
			{
				if (!params.colorBuffers[i].HasSameExtent(params.colorBuffers[i - 1])) 
					return nullptr;
				// Log that all image views must have same extent
			}
		}
		auto framebuffer = MakeRef<VulkanFramebuffer>(std::move(params), this);
		return framebuffer;
	}

	RefPtr<IGPURenderPass> VulkanDevice::CreateRenderPass(const IFramebuffer::Layout& params)
	{
		return MakeRef<VulkanRenderPass>(this, params);
	}

	uint32_t VulkanDevice::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDeviceMemoryProperties memProps{};
		vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProps);

		for (int i = 0; i < memProps.memoryTypeCount; ++i)
		{
			if ((typeFilter & (1 << i)) && ((memProps.memoryTypes[i].propertyFlags & properties) == properties))
			{
				return i;
			}
		}
	}

	VulkanRenderPass* VulkanDevice::ObtainDummyRenderPass(const IFramebuffer::Layout& layout)
	{
		{
			std::shared_lock lock(m_dummyRPsMutex);
			if (m_dummyRPs.find(layout) != m_dummyRPs.end())
				return m_dummyRPs[layout].get();
		}
		std::unique_lock uniqueLock(m_dummyRPsMutex);
		if (m_dummyRPs.find(layout) != m_dummyRPs.end())
			return m_dummyRPs[layout].get();
		auto rp = MakeRef<VulkanRenderPass>(this, layout);
		m_dummyRPs[layout] = rp;
		return rp.get();
	}

	RefPtr<IGPUImage> VulkanDevice::GetBackbuffer()
	{
		return m_swapchain->GetBackBuffer();
	}

	RefPtr<IFence> VulkanDevice::CreateFence(bool createSignaled)
	{
		return MakeRef<VulkanFence>(this, createSignaled);
	}

	RefPtr<IGPUBuffer> VulkanDevice::CreateBuffer(const IGPUBuffer::CreationParams& params)
	{
		auto buffer = MakeRef<VulkanGPUBuffer>(this, params);
		return buffer;
	}

	ImageFormat VulkanDevice::GetSwapchainFormat()
	{
		return VulkanConstantTranslator::GetEngineImageFormat(m_swapchain->GetFormat());
	}

	void VulkanDevice::Submit(ICommandBuffer* cb)
	{
		m_graphicsQueue->Submit(cb);
	}

	void VulkanDevice::Present()
	{
		m_presentQueue->PresentSwapchainImage(m_swapchain.get(), m_graphicsQueue.get());
	}

	void VulkanDevice::BeginFrame()
	{
		m_graphicsQueue->GetFence().Wait();
		m_graphicsQueue->GetFence().Reset();
		m_commandPools[m_swapchain->GetImageIndex()].WaitCommandBuffersAndClear();
		m_swapchain->GetNextImage();
	}


	bool VulkanDevice::Deinit()
	{

		vkDestroyDevice(m_device, nullptr);
		vkDestroyInstance(m_instance, nullptr);

		return true;
	}
}