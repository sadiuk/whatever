#pragma once
#include "IGPUBuffer.h"
#include "VulkanDevice.h"
#include "vulkan/vulkan.h"
#include "VulkanConstantTranslator.h"
#include "VkMakros.h"
namespace wtv
{
	class VulkanGPUBuffer : public IGPUBuffer, public IServiceProviderHolder
	{
	public:
		VulkanGPUBuffer(VulkanDevice* engine, const CreationParams& params);
		~VulkanGPUBuffer();
		VkBuffer GetNativeHandle() { return m_buffer; }
		IServiceProvider* GetServiceProvider() { return m_engine->GetServiceProvider(); }
	private:
		VulkanDevice* m_engine;
		VkBuffer m_buffer;
		VkDeviceMemory m_memory;
	};
}