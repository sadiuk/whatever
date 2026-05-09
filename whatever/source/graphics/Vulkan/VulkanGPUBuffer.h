#pragma once
#include "graphics/IGPUBuffer.h"
#include "vulkan/vulkan.h"
#include "VulkanConstantTranslator.h"
#include "VkMakros.h"
#include "IServiceProvider.h"


namespace wtv
{
	class VulkanDevice;
	struct IVulkanAllocator;
	class VulkanGPUBuffer : public IGPUBuffer, public IServiceProviderHolder
	{
	public:
		VulkanGPUBuffer(VulkanDevice* engine, const CreationParams& params, const std::string& name, IVulkanAllocator* allocator);
		~VulkanGPUBuffer();
		VkBuffer GetNativeHandle() { return m_buffer; }
		IServiceProvider* GetServiceProvider() const override;
		void* Map() override;
		const void* Map() const override;
		void Unmap() override;
	private:
		VulkanDevice* m_device;
		VkBuffer m_buffer;
		VkDeviceMemory m_memory;
		IVulkanAllocator* m_allocator;
	};
}