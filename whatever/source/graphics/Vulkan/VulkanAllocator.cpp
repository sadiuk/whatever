#include "VulkanAllocator.h"

namespace wtv
{
	VulkanAllocator::Page::Page(VulkanDevice* device, VkMemoryPropertyFlags memoryPropFlags, size_t pageSize) :
		m_device(device),
		m_pageSize(pageSize)
	{
		auto memoryTypeIndex = m_device->GetMemoryTypeIndex(memoryPropFlags);
		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = m_pageSize;
		allocInfo.memoryTypeIndex = memoryTypeIndex;
		vkAllocateMemory(m_device->GetDevice(), &allocInfo, nullptr, &m_memory);

		m_freeBlocks.emplace_back(0, m_pageSize);
	}

	bool VulkanAllocator::Page::AllocateBuffer(VulkanGPUBuffer* buffer)
	{
		auto bufferDesc = buffer->GetProperties();
		size_t size = bufferDesc.bufferSize;
		size_t alignment = bufferDesc.alignment;

		if (!CanAllocateContiguous(size, alignment))
		{
			return false;
		}
		for (auto it = m_freeBlocks.begin(); it != m_freeBlocks.end(); ++it)
		{
			size_t alignedOffset = (it->first + alignment - 1) & ~(alignment - 1);
			if (alignedOffset + size <= it->first + it->second)
			{
				size_t padding = alignedOffset - it->first;
				size_t remainingSize = it->second - padding - size;
				if (padding > 0)
				{
					it->second = padding;
				}
				else
				{
					m_freeBlocks.erase(it);
				}
				if (remainingSize > 0)
				{
					m_freeBlocks.emplace_back(alignedOffset + size, remainingSize);
				}

				VkBindBufferMemoryInfo bindInfo{};
				bindInfo.sType = VK_STRUCTURE_TYPE_BIND_BUFFER_MEMORY_INFO;
				bindInfo.buffer = buffer->GetNativeHandle();
				bindInfo.memory = m_memory;
				bindInfo.memoryOffset = alignedOffset;
				VkResult res = vkBindBufferMemory2(m_device->GetDevice(), 1, &bindInfo);

				if (res == VK_SUCCESS)
						return true;
			}

		}

		return false;
	}
}