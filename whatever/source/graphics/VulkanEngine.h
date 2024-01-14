#pragma once
#include "IEngine.h"
#include "IVulkanSurface.h" 
#include <ui/IWindow.h>

#include "vulkan/vulkan.hpp"
#include "vma/vk_mem_alloc.h"

#include <optional>

#ifdef _DEBUG
#define USE_VALIDATION_LAYERS true
#else

#endif

class VulkanEngine : public IEngine
{
	static constexpr uint32_t VULKAN_VERSION = VK_API_VERSION_1_2;

	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamilyIndex{};
		std::optional<uint32_t> computeFamilyIndex{};
		bool IsValid() { return graphicsFamilyIndex.has_value() && computeFamilyIndex.has_value(); }
	};
public:
	VulkanEngine(const std::string& appName, ISurfaceFactory* factory);
	~VulkanEngine();
	GRAPHICS_API GetAPI() override { return VULKAN; }
private:
	bool Init(const std::string& appName);
	bool CreateInstance(const std::string& appName);
	bool SelectPhysicalDevice();
	bool CreateLogicalDevice();
	bool CreateAllocator();
	bool InitSurface();
	bool EnsureValidationLayersAvailable(std::vector<const char*> requestedLayers);

	bool Deinit();
private:
	VkInstance m_instance;
	VkPhysicalDevice m_physicalDevice;
	VkDevice m_device;

	VmaAllocator m_allocator;
	QueueFamilyIndices m_queueFamilyIndices;
	
	std::shared_ptr<IVulkanSurface> m_surface;
};