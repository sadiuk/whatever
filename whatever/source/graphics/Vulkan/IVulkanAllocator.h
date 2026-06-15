#pragma once
#include "graphics/IGPUBuffer.h"
#include "graphics/IGPUImage.h"
#include "util/RefPtr.h"

namespace wtv
{
	struct IVulkanAllocator : IReferenceCounted
	{
		virtual VkBuffer AllocateBuffer(const IGPUBuffer::CreationParams& params) = 0;
		virtual VkImage AllocateImage(const IImage::CreationParams& createInfo, MemoryPropertyFlags memoryFlags) = 0;
		virtual void DeallocateBuffer(VkBuffer buffer) = 0;
		virtual void DeallocateImage(VkImage buffer) = 0;
		virtual void* Map(VkBuffer buffer)	= 0;
		virtual const void* Map(VkBuffer buffer) const= 0;
		virtual void Unmap(VkBuffer buffer) = 0;
	};
}