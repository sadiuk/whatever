#pragma once
#include "GraphicsConstants.h"
#include "IImage.h"

#include <cstdint>
#include <type_traits>
namespace wtv
{
	struct IGPUImage : public IImage
	{
		IGPUImage(const CreationParams& params) : m_props(params)
		{
		}
		const Properties& GetProperties() const override { return m_props; }
		virtual ~IGPUImage() {}
	protected:
		Properties m_props;
	};
}