#pragma once
#include "IGPUBuffer.h"
#include "VulkanEngine.h"
#include "vulkan/vulkan.h"
#include "VulkanConstantTranslator.h"
#include "VkMakros.h"
namespace wtv
{
	class VulkanGPUBuffer : public IGPUBuffer
	{
	public:
		VulkanGPUBuffer(VulkanEngine* engine, const CreationParams& params);
		~VulkanGPUBuffer();
		VkBuffer GetNativeHandle() { return m_buffer; }
	private:
		VulkanEngine* m_engine;
		VkBuffer m_buffer;
		VkDeviceMemory m_memory;
	};
}