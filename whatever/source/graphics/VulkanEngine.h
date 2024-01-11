#pragma once
#include "IEngine.h"
#include "vulkan/vulkan.hpp"
#include <optional>

#ifdef _DEBUG
#define USE_VALIDATION_LAYERS true
#else

#endif

class VulkanEngine : public IEngine
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamilyIndex{};
		std::optional<uint32_t> computeFamilyIndex{};
	};
public:
	VulkanEngine(const std::string& appName);
private:
	bool Init(const std::string& appName);
	bool CreateInstance(const std::string& appName);
	bool CreatePhysicalDevice();
	bool CreateLogicalDevice();
	bool EnsureValidationLayersAvailable(std::vector<const char*> requestedLayers);
private:
	VkInstance m_instance;
	VkPhysicalDevice m_physicalDevice;

	QueueFamilyIndices m_queueFamilyIndices;

};