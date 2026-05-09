#pragma once
#include <vector>
#include <utility>

#include "graphics/Vulkan/VulkanDevice.h"
#include "graphics/Vulkan/VulkanGPUBuffer.h"
#include "IVulkanAllocator.h"
#include "VulkanGPUBuffer.h"
#include "vulkan/vulkan.h"
namespace wtv
{

	class VulkanAllocator : public IVulkanAllocator
	{
		class Page
		{
		private:
			VulkanDevice* m_device;
			VkDeviceMemory m_memory;
			std::vector<std::pair<size_t, size_t>> m_freeBlocks;
			size_t m_pageSize;
		public:
			Page(VulkanDevice* device, VkMemoryPropertyFlags memoryPropFlags, size_t pageSize);
			bool CanAllocateContiguous(size_t size, size_t alignment)
			{
				for(const auto& block : m_freeBlocks)
				{
					size_t alignedOffset = (block.first + alignment - 1) & ~(alignment - 1);
					if (alignedOffset + size <= block.first + block.second)
					{
						return true;
					}
				}
				return false;
			}

			bool HasFreeBlocks() const
			{
				return !m_freeBlocks.empty();
			}
			
			bool AllocateBuffer(VulkanGPUBuffer* buffer);
			
		};

	public:
		VulkanAllocator(VulkanDevice* device) : m_device(device) 
		{
			assert(false);
			//m_pages.emplace_back(device);
		}
		VkBuffer AllocateBuffer(const IGPUBuffer::CreationParams& createInfo) override { assert(false); return VK_NULL_HANDLE; }
		void DeallocateBuffer(VkBuffer buffer) override { assert(false); return; }
	private:
		std::vector<Page> m_pages;
		VulkanDevice* m_device;
	};
}