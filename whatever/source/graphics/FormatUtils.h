#pragma once
#include "GraphicsConstants.h"

namespace wtv
{
	inline uint32_t BytesPerPixel(ImageFormat format)
	{
		switch (format)
		{
        case ImageFormat::R4G4_UNORM_PACK8: return 1;
        case ImageFormat::R4G4B4A4_UNORM_PACK16: return 2;
        case ImageFormat::B4G4R4A4_UNORM_PACK16: return 2;
        case ImageFormat::R5G6B5_UNORM_PACK16: return 2;
        case ImageFormat::B5G6R5_UNORM_PACK16: return 2;
        case ImageFormat::R5G5B5A1_UNORM_PACK16: return 2;
        case ImageFormat::B5G5R5A1_UNORM_PACK16: return 2;
        case ImageFormat::A1R5G5B5_UNORM_PACK16: return 2;
        case ImageFormat::R8_UNORM: return 1;
        case ImageFormat::R8_SNORM: return 1;
        case ImageFormat::R8_USCALED: return 1;
        case ImageFormat::R8_SSCALED: return 1;
        case ImageFormat::R8_UINT: return 1;
        case ImageFormat::R8_SINT: return 1;
        case ImageFormat::R8_SRGB: return 1;
        case ImageFormat::R8G8_UNORM: return 2;
        case ImageFormat::R8G8_SNORM: return 2;
        case ImageFormat::R8G8_USCALED: return 2;
        case ImageFormat::R8G8_SSCALED: return 2;
        case ImageFormat::R8G8_UINT: return 2;
        case ImageFormat::R8G8_SINT: return 2;
        case ImageFormat::R8G8_SRGB: return 2;
        case ImageFormat::R8G8B8_UNORM: return 3;
        case ImageFormat::R8G8B8_SNORM: return 3;
        case ImageFormat::R8G8B8_USCALED: return 3;
        case ImageFormat::R8G8B8_SSCALED: return 3;
        case ImageFormat::R8G8B8_UINT: return 3;
        case ImageFormat::R8G8B8_SINT: return 3;
        case ImageFormat::R8G8B8_SRGB: return 3;
        case ImageFormat::B8G8R8_UNORM: return 3;
        case ImageFormat::B8G8R8_SNORM: return 3;
        case ImageFormat::B8G8R8_USCALED: return 3;
        case ImageFormat::B8G8R8_SSCALED: return 3;
        case ImageFormat::B8G8R8_UINT: return 3;
        case ImageFormat::B8G8R8_SINT: return 3;
        case ImageFormat::B8G8R8_SRGB: return 3;
        case ImageFormat::R8G8B8A8_UNORM: return 4;
        case ImageFormat::R8G8B8A8_SNORM: return 4;
        case ImageFormat::R8G8B8A8_USCALED: return 4;
        case ImageFormat::R8G8B8A8_SSCALED: return 4;
        case ImageFormat::R8G8B8A8_UINT: return 4;
        case ImageFormat::R8G8B8A8_SINT: return 4;
        case ImageFormat::R8G8B8A8_SRGB: return 4;
        case ImageFormat::B8G8R8A8_UNORM: return 4;
        case ImageFormat::B8G8R8A8_SNORM: return 4;
        case ImageFormat::B8G8R8A8_USCALED: return 4;
        case ImageFormat::B8G8R8A8_SSCALED: return 4;
        case ImageFormat::B8G8R8A8_UINT: return 4;
        case ImageFormat::B8G8R8A8_SINT: return 4;
        case ImageFormat::B8G8R8A8_SRGB: return 4;
        case ImageFormat::A8B8G8R8_UNORM_PACK32: return 4;
        case ImageFormat::A8B8G8R8_SNORM_PACK32: return 4;
        case ImageFormat::A8B8G8R8_USCALED_PACK32: return 4;
        case ImageFormat::A8B8G8R8_SSCALED_PACK32: return 4;
        case ImageFormat::A8B8G8R8_UINT_PACK32: return 4;
        case ImageFormat::A8B8G8R8_SINT_PACK32: return 4;
        case ImageFormat::A8B8G8R8_SRGB_PACK32: return 4;
        case ImageFormat::A2R10G10B10_UNORM_PACK32: return 4;
        case ImageFormat::A2R10G10B10_SNORM_PACK32: return 4;
        case ImageFormat::A2R10G10B10_USCALED_PACK32: return 4;
        case ImageFormat::A2R10G10B10_SSCALED_PACK32: return 4;
        case ImageFormat::A2R10G10B10_UINT_PACK32: return 4;
        case ImageFormat::A2R10G10B10_SINT_PACK32: return 4;
        case ImageFormat::A2B10G10R10_UNORM_PACK32: return 4;
        case ImageFormat::A2B10G10R10_SNORM_PACK32: return 4;
        case ImageFormat::A2B10G10R10_USCALED_PACK32: return 4;
        case ImageFormat::A2B10G10R10_SSCALED_PACK32: return 4;
        case ImageFormat::A2B10G10R10_UINT_PACK32: return 4;
        case ImageFormat::A2B10G10R10_SINT_PACK32: return 4;
        case ImageFormat::R16_UNORM: return 2;
        case ImageFormat::R16_SNORM: return 2;
        case ImageFormat::R16_USCALED: return 2;
        case ImageFormat::R16_SSCALED: return 2;
        case ImageFormat::R16_UINT: return 2;
        case ImageFormat::R16_SINT: return 2;
        case ImageFormat::R16_SFLOAT: return 2;
        case ImageFormat::R16G16_UNORM: return 4;
        case ImageFormat::R16G16_SNORM: return 4;
        case ImageFormat::R16G16_USCALED: return 4;
        case ImageFormat::R16G16_SSCALED: return 4;
        case ImageFormat::R16G16_UINT: return 4;
        case ImageFormat::R16G16_SINT: return 4;
        case ImageFormat::R16G16_SFLOAT: return 4;
        case ImageFormat::R16G16B16_UNORM: return 6;
        case ImageFormat::R16G16B16_SNORM: return 6;
        case ImageFormat::R16G16B16_USCALED: return 6;
        case ImageFormat::R16G16B16_SSCALED: return 6;
        case ImageFormat::R16G16B16_UINT: return 6;
        case ImageFormat::R16G16B16_SINT: return 6;
        case ImageFormat::R16G16B16_SFLOAT: return 6;
        case ImageFormat::R16G16B16A16_UNORM: return 8;
        case ImageFormat::R16G16B16A16_SNORM: return 8;
        case ImageFormat::R16G16B16A16_USCALED: return 8;
        case ImageFormat::R16G16B16A16_SSCALED: return 8;
        case ImageFormat::R16G16B16A16_UINT: return 8;
        case ImageFormat::R16G16B16A16_SINT: return 8;
        case ImageFormat::R16G16B16A16_SFLOAT: return 8;
        case ImageFormat::R32_UINT: return 4;
        case ImageFormat::R32_SINT: return 4;
        case ImageFormat::R32_SFLOAT: return 4;
        case ImageFormat::R32G32_UINT: return 8;
        case ImageFormat::R32G32_SINT: return 8;
        case ImageFormat::R32G32_SFLOAT: return 8;
        case ImageFormat::R32G32B32_UINT: return 12;
        case ImageFormat::R32G32B32_SINT: return 12;
        case ImageFormat::R32G32B32_SFLOAT: return 12;
        case ImageFormat::R32G32B32A32_UINT: return 16;
        case ImageFormat::R32G32B32A32_SINT: return 16;
        case ImageFormat::R32G32B32A32_SFLOAT: return 16;
        case ImageFormat::D16_UNORM: return 2;
        case ImageFormat::D32_SFLOAT: return 4;
        case ImageFormat::S8_UINT: return 1;
        case ImageFormat::D16_UNORM_S8_UINT: return 3; // Assuming D16_UNORM (2 bytes) + S8_UINT (1 byte)
        case ImageFormat::D24_UNORM_S8_UINT: return 4; // Assuming D24_UNORM (3 bytes) + S8_UINT (1 byte)
        case ImageFormat::D32_SFLOAT_S8_UINT: return 5; // Assuming D32_SFLOAT (4 bytes) + S8_UINT (1 byte)
        case ImageFormat::Undefined:
        default:
            assert(false);
            return 0;
		}
	}

    inline bool IsDepthFormat(ImageFormat format)
    {
        switch (format)
        {
        case ImageFormat::D16_UNORM:
        case ImageFormat::D32_SFLOAT:
        case ImageFormat::D16_UNORM_S8_UINT:
        case ImageFormat::D24_UNORM_S8_UINT:
        case ImageFormat::D32_SFLOAT_S8_UINT:
            return true;
        default:
            return false;
        }
	}
}