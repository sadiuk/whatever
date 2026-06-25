#include "VulkanGPUBuffer.h"
#include "VulkanDevice.h"
#include "IVulkanAllocator.h"

namespace wtv
{
	VulkanGPUBuffer::VulkanGPUBuffer(VulkanDevice* device, const CreationParams& params, const std::string& name, IVulkanAllocator* allocator) : 
		IGPUBuffer(params, name), 
		m_device(device),
		m_allocator(allocator)
	{
		m_buffer = m_allocator->AllocateBuffer(params);

		m_device->GetDebugNamer().SetResourceName(m_buffer, VK_OBJECT_TYPE_BUFFER, name.c_str());
	}

	VulkanGPUBuffer::~VulkanGPUBuffer()
	{
		uint64_t semaphoreWaitValue = GetSemaphoreWaitValue();
		VkBuffer buffer = m_buffer;
		IVulkanAllocator* allocator = m_allocator;
		m_device->EnqueueForDeletion([semaphoreWaitValue, buffer, allocator](uint64_t completedValue) {
			if (completedValue >= semaphoreWaitValue)
			{
				allocator->DeallocateBuffer(buffer);
				return true;
			}
			return false;
		});
	}

	IServiceProvider* VulkanGPUBuffer::GetServiceProvider() const
	{
		return m_device->GetServiceProvider();
	}

	void* VulkanGPUBuffer::Map()
	{
		return m_allocator->Map(m_buffer);
	}

	const void* VulkanGPUBuffer::Map() const
	{
		return m_allocator->Map(m_buffer);
	}
	
	void VulkanGPUBuffer::Unmap()
	{
		m_allocator->Unmap(m_buffer);
	}
}