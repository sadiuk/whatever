#pragma once
#include "GraphicsConstants.h"
#include "IImage.h"
#include "IGPUResource.h"

#include <cstdint>
#include <type_traits>
namespace wtv
{
	struct IGPUImage : public IImage, public IGPUResource, public IGPUMemoryResource
	{
		IGPUImage(const CreationParams& params) : m_props(params)
		{
		}
		GPUResourceType GetResourceType() const
		{
			return GPUResourceType::Image;
		}
		const Properties& GetProperties() const override { return m_props; }
		virtual ~IGPUImage() {}
	protected:
		Properties m_props;
	};
}