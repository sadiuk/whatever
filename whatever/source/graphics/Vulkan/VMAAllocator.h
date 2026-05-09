#pragma once
#include "IServiceProvider.h"
#include "IVulkanAllocator.h"
#include "vma/include/vk_mem_alloc.h"


namespace wtv
{
	class VulkanDevice;
	class VMAAllocator : public IVulkanAllocator, public IServiceProviderHolder
	{
		VulkanDevice* m_device;
		VmaAllocator m_allocator;
		IServiceProvider* m_services;

		std::unordered_map<VkBuffer, VmaAllocation> m_allocationsMap;
	public:
		VMAAllocator(VulkanDevice* device, IServiceProvider* services);
		VkBuffer AllocateBuffer(const IGPUBuffer::CreationParams& createInfo) override;
		void DeallocateBuffer(VkBuffer buffer) override;
		IServiceProvider* GetServiceProvider() const override { return m_services; }

		void* Map(VkBuffer buffer) override;
		const void* Map(VkBuffer buffer) const override { return Map(buffer); }
		void Unmap(VkBuffer buffer) override;

	};
}