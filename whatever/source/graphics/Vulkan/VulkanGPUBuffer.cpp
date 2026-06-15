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

		m_device->GetDebugNamer().SetBufferName(m_buffer, name.c_str());
	}

	VulkanGPUBuffer::~VulkanGPUBuffer()
	{
		m_device->EnqueueForDeletion(m_buffer, GetSemaphoreWaitValue());
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