#pragma once
#include "IBuffer.h"
#include "IGPUResource.h"
#include "Vulkan/VulkanConstantTranslator.h"
#include <string>
namespace wtv
{
	struct IGPUBuffer : public IBuffer, public IGPUResource, public IGPUMemoryResource
	{
		IGPUBuffer(const CreationParams& createInfo, std::string name) : IBuffer(createInfo, name) {}
		GPUResourceType GetResourceType() const
		{
			return GPUResourceType::Buffer;
		}
		virtual ~IGPUBuffer() = default;
		virtual void* Map() = 0;
		virtual const void* Map() const = 0;
		virtual void Unmap() = 0;
	protected:
	};
}