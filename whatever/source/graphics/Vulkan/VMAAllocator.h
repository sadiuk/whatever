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

		std::unordered_map<VkBuffer, VmaAllocation> m_bufferAllocationsMap;
		std::unordered_map<VkImage, VmaAllocation> m_imageAllocationsMap;
	public:
		VMAAllocator(VulkanDevice* device, IServiceProvider* services);
		VkBuffer AllocateBuffer(const IGPUBuffer::CreationParams& createInfo) override;
		VkImage AllocateImage(const IImage::CreationParams& createInfo, MemoryPropertyFlags memoryFlags) override;
		void DeallocateBuffer(VkBuffer buffer) override;
		void DeallocateImage(VkImage Image) override;
		IServiceProvider* GetServiceProvider() const override { return m_services; }

		void* Map(VkBuffer buffer) override;
		const void* Map(VkBuffer buffer) const override { return Map(buffer); }
		void Unmap(VkBuffer buffer) override;

	};
}