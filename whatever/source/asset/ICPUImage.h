#pragma once
#include "graphics/IImage.h"
#include "util/RefPtr.h"
#include "graphics/FormatUtils.h"
#include <vector>
#include <string>

namespace wtv
{
	struct CPUImageData : public IReferenceCounted
	{
		CPUImageData() {}
		CPUImageData(const CPUImageData& other) :
			data(other.data),
			width(other.width),
			height(other.height),
			depth(other.depth),
			arraySize(other.arraySize),
			mipLevels(other.mipLevels),
			format(other.format),
			dimension(other.dimension),
			samples(other.samples)
		{}
		std::vector<char> data;
		uint32_t width = 0;
		uint32_t height = 0;
		uint32_t depth = 1;
		uint32_t arraySize = 1;
		uint32_t mipLevels = 1;
		ImageFormat format = ImageFormat::Undefined;
		ImageDimension dimension = ImageDimension::Dimension2D;
		uint32_t samples = 1;
		std::string name = "";

		bool IsValid() const
		{
			return !data.empty() && format != ImageFormat::Undefined && width != 0 && height != 0;
		}

		uint64_t CalculateRequiredStagingSize() const
		{
			return (uint64_t)width * (uint64_t)height * (uint64_t)depth * arraySize * mipLevels * GetPixelSize(format);
		}
	};
}