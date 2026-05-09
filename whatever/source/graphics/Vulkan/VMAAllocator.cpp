#include "VMAAllocator.h"
#include "VulkanDevice.h"

namespace wtv
{
	VMAAllocator::VMAAllocator(VulkanDevice* device, IServiceProvider* services) :
		m_device(device),
		m_services(services)
	{
		VmaAllocatorCreateInfo allocatorCreateInfo = {};
		allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_2; // Ваша версія API
		allocatorCreateInfo.physicalDevice = m_device->GetPhysicalDevice();
		allocatorCreateInfo.device = m_device->GetDevice();
		allocatorCreateInfo.instance = m_device->GetInstance();

		ASSERT_VK_SUCCESS(vmaCreateAllocator(&allocatorCreateInfo, &m_allocator));

	}

	VkBuffer VMAAllocator::AllocateBuffer(const IGPUBuffer::CreationParams& params)
	{
		auto device = m_device->GetDevice();
		auto physDevice = m_device->GetPhysicalDevice();


		auto indices = m_device->GetQueueFamilyIndices().Vector();
		VkBufferCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.size = params.bufferSize;
		createInfo.pQueueFamilyIndices = indices.data();
		createInfo.queueFamilyIndexCount = (uint32_t)indices.size();
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.usage = VulkanConstantTranslator::GetVkBufferUsageFlags((uint32_t)params.usageFlags);
		//TODO: ditch this, must be done manually
		createInfo.usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		createInfo.usage |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

		VmaAllocationCreateInfo allocCreateInfo = {};
		allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
		if ((params.memoryFlags & MemoryPropertyFlags::HostVisible) == MemoryPropertyFlags::HostVisible)
			allocCreateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
		else
			allocCreateInfo.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
		allocCreateInfo.priority = 1.0f;

		VkBuffer buffer;
		VmaAllocation allocation;
		vmaCreateBuffer(m_allocator, &createInfo, &allocCreateInfo, &buffer, &allocation, nullptr);
		
		ASSERT_TRUE_ELSE_RET0(m_allocationsMap.find(buffer) == m_allocationsMap.end());
		m_allocationsMap[buffer] = allocation;
		return buffer;
	}

	void VMAAllocator::DeallocateBuffer(VkBuffer buffer)
	{
		ASSERT_TRUE_ELSE_RET(m_allocationsMap.find(buffer) != m_allocationsMap.end())
		VmaAllocation allocation = m_allocationsMap[buffer];

		vmaDestroyBuffer(m_allocator, buffer, allocation);
	}

	void* VMAAllocator::Map(VkBuffer buffer)
	{
		void* data;
		VmaAllocation allocation = m_allocationsMap[buffer];
		vmaMapMemory(m_allocator, allocation, &data);
		return data;
	}

	void VMAAllocator::Unmap(VkBuffer buffer)
	{
		VmaAllocation allocation = m_allocationsMap[buffer];
		vmaUnmapMemory(m_allocator, allocation);
	}
}
