#include "util/RefPtr.h"
#include "GraphicsConstants.h"
namespace wtv
{
	struct IImage : IReferenceCounted
	{
		struct CreationParams
		{
			uint32_t width;
			uint32_t height;
			uint32_t depth = 1;
			uint32_t arraySize = 1;
			uint32_t mipLevels = 1;
			ImageFormat format;
			ImageDimension dimension = ImageDimension::Dimension2D;
			uint32_t samples = 1;
			std::underlying_type_t<ImageUsage> usageFlags;
		};
		struct Properties
		{
			Properties(const CreationParams& params) :
				width(params.width),
				height(params.height),
				depth(params.depth),
				arraySize(params.arraySize),
				mipLevels(params.mipLevels),
				format(params.format),
				dimension(params.dimension),
				samples(params.samples)	{}
			uint32_t width;
			uint32_t height;
			uint32_t depth = 1;
			uint32_t arraySize = 1;
			uint32_t mipLevels = 1;
			ImageFormat format;
			ImageDimension dimension = ImageDimension::Dimension2D;
			uint32_t samples = 1;
		};
		struct View
		{
			View(IImage* img) : image(img)
			{
				const auto& props = image->GetProperties();
				width = props.width;
				height = props.height;
				depth = props.depth;
			}
			IImage* image;
			uint32_t x = 0;
			uint32_t y = 0;
			uint32_t z = 0;
			uint32_t width;
			uint32_t height;
			uint32_t depth = 1;
			uint32_t baseMip = 0;
			uint32_t mipCount = 1;
			uint32_t baseArrayLayer = 0;
			uint32_t arrayLayerCount = 1;
			bool operator ==(const View& other) const = default;
			bool HasSameExtent(const View& other) const
			{
				return width == other.width &&
					height == other.height &&
					depth == other.depth &&
					baseMip == other.baseMip &&
					mipCount == other.mipCount &&
					baseArrayLayer == other.baseArrayLayer &&
					arrayLayerCount == other.arrayLayerCount;
			}
		};

		virtual const Properties& GetProperties() const = 0;
	};
}