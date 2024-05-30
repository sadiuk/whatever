#pragma once
#include "IBuffer.h"
#include "VulkanConstantTranslator.h"
#include <string>
namespace wtv
{
	struct IGPUBuffer : public IBuffer
	{
		IGPUBuffer(const CreationParams& createInfo) : IBuffer(createInfo) {}
		virtual ~IGPUBuffer() = default;
	protected:
	};
}