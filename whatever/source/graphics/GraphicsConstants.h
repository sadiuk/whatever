#pragma once
#include <cassert>
#include <cstdint>
#include <climits>

namespace wtv
{
    enum class ShaderStage : uint16_t
    {
        Vertex = 0,
        Geometry,
        Fragment,
        GraphicsStageCount = Fragment + 1,


        Compute = GraphicsStageCount,
        Count
    };

    enum ShaderStageFlags
    {
        Vertex = 1,
        Geometry = 2,
        Fragment = 4,

        All = INT_MAX
    };

    enum class VertexAtributeType : uint16_t
    {
        int1,
        int2,
        int3,
        int4,
        uint1,
        uint2,
        uint3,
        uint4,
        float1,
        float2,
        float3,
        float4,
    };

    enum class PrimitiveTopology
    {
        PointList = 0,
        LineList = 1,
        LineStrip = 2,
        TriangleList = 3,
        TriangleStrip = 4,
        TriangleFan = 5,
        LineListWithAdjacency = 6,
        LineStripWithAdjacency = 7,
        TriangleListWithAdjacency = 8,
        TriangleStripWithAdjacency = 9,
        PatchList = 10
    };

    enum class FrontFace
    {
        CounterClockwise,
        Clockwise,
        Undefined
    };

    enum class CullMode
    {
        None,
        Front,
        Back,
        FrontAndBack,
        Undefined
    };

    enum class PolygonMode
    {
        Fill,
        Line,
        Point,
        Undefined
    };

    enum class CompareOperation {
        Never,
        Less,
        Equal,
        LessOrEqual,
        Greater,
        NotEqual,
        GreaterOrEqual,
        Always,
        Undefined
    };

    enum class StencilTestOperation {
        Keep,
        Zero,
        Replace,
        IncrementAndClamp,
        DecrementAndClamp,
        Invert,
        IncrementAndWrap,
        DecrementAndWrap,
        Undefined
    };


    enum class BlendFactor
    {
        Zero = 0,
        One,
        SrcColor,
        OneMinusSrcColor,
        DstColor,
        OneMinusDstColor,
        SrcAlpha,
        OneMinusSrcAlpha,
        DstAlpha,
        OneMinusDstAlpha,
        ConstantColor,
        OneMinusConstantColor,
        ConstantAlpha,
        OneMinusConstantAlpha,
        SrcAlphaSaturate,
        Src1Color,
        OneMinusSrc1Color,
        Src1Alpha,
        OneMinusSrc1Alpha,
        Undefined
    };

    enum class BlendOperation
    {
        Add = 1,
        Subtract,
        ReverseSubtract,
        Min,
        Max,
        Undefined
    };

    enum class ImageFormat
    {
        R4G4_UNORM_PACK8 = 1,
        R4G4B4A4_UNORM_PACK16,
        B4G4R4A4_UNORM_PACK16,
        R5G6B5_UNORM_PACK16,
        B5G6R5_UNORM_PACK16,
        R5G5B5A1_UNORM_PACK16,
        B5G5R5A1_UNORM_PACK16,
        A1R5G5B5_UNORM_PACK16,
        R8_UNORM,
        R8_SNORM,
        R8_USCALED,
        R8_SSCALED,
        R8_UINT,
        R8_SINT,
        R8_SRGB,
        R8G8_UNORM,
        R8G8_SNORM,
        R8G8_USCALED,
        R8G8_SSCALED,
        R8G8_UINT,
        R8G8_SINT,
        R8G8_SRGB,
        R8G8B8_UNORM,
        R8G8B8_SNORM,
        R8G8B8_USCALED,
        R8G8B8_SSCALED,
        R8G8B8_UINT,
        R8G8B8_SINT,
        R8G8B8_SRGB,
        B8G8R8_UNORM,
        B8G8R8_SNORM,
        B8G8R8_USCALED,
        B8G8R8_SSCALED,
        B8G8R8_UINT,
        B8G8R8_SINT,
        B8G8R8_SRGB,
        R8G8B8A8_UNORM,
        R8G8B8A8_SNORM,
        R8G8B8A8_USCALED,
        R8G8B8A8_SSCALED,
        R8G8B8A8_UINT,
        R8G8B8A8_SINT,
        R8G8B8A8_SRGB,
        B8G8R8A8_UNORM,
        B8G8R8A8_SNORM,
        B8G8R8A8_USCALED,
        B8G8R8A8_SSCALED,
        B8G8R8A8_UINT,
        B8G8R8A8_SINT,
        B8G8R8A8_SRGB,
        A8B8G8R8_UNORM_PACK32,
        A8B8G8R8_SNORM_PACK32,
        A8B8G8R8_USCALED_PACK32,
        A8B8G8R8_SSCALED_PACK32,
        A8B8G8R8_UINT_PACK32,
        A8B8G8R8_SINT_PACK32,
        A8B8G8R8_SRGB_PACK32,
        A2R10G10B10_UNORM_PACK32,
        A2R10G10B10_SNORM_PACK32,
        A2R10G10B10_USCALED_PACK32,
        A2R10G10B10_SSCALED_PACK32,
        A2R10G10B10_UINT_PACK32,
        A2R10G10B10_SINT_PACK32,
        A2B10G10R10_UNORM_PACK32,
        A2B10G10R10_SNORM_PACK32,
        A2B10G10R10_USCALED_PACK32,
        A2B10G10R10_SSCALED_PACK32,
        A2B10G10R10_UINT_PACK32,
        A2B10G10R10_SINT_PACK32,
        R16_UNORM,
        R16_SNORM,
        R16_USCALED,
        R16_SSCALED,
        R16_UINT,
        R16_SINT,
        R16_SFLOAT,
        R16G16_UNORM,
        R16G16_SNORM,
        R16G16_USCALED,
        R16G16_SSCALED,
        R16G16_UINT,
        R16G16_SINT,
        R16G16_SFLOAT,
        R16G16B16_UNORM,
        R16G16B16_SNORM,
        R16G16B16_USCALED,
        R16G16B16_SSCALED,
        R16G16B16_UINT,
        R16G16B16_SINT,
        R16G16B16_SFLOAT,
        R16G16B16A16_UNORM,
        R16G16B16A16_SNORM,
        R16G16B16A16_USCALED,
        R16G16B16A16_SSCALED,
        R16G16B16A16_UINT,
        R16G16B16A16_SINT,
        R16G16B16A16_SFLOAT,
        R32_UINT,
        R32_SINT,
        R32_SFLOAT,
        R32G32_UINT,
        R32G32_SINT,
        R32G32_SFLOAT,
        R32G32B32_UINT,
        R32G32B32_SINT,
        R32G32B32_SFLOAT,
        R32G32B32A32_UINT,
        R32G32B32A32_SINT,
        R32G32B32A32_SFLOAT,
        D16_UNORM,
        D32_SFLOAT,
        S8_UINT,
        D16_UNORM_S8_UINT,
        D24_UNORM_S8_UINT,
        D32_SFLOAT_S8_UINT,
        Undefined
    };

    enum class ImageDimension
    {
        Dimension1D,
        Dimension2D,
        Dimension3D,
        Undefined
    };

    enum class ImageUsage : uint32_t
    {
        Undefined = 0,
        CopySource = 1,
        CopyDestination = CopySource << 1,
        SampledImage = CopyDestination << 1,
        StorageImage = SampledImage << 1,
        ColorAttachment = StorageImage << 1,
        DepthStencilAttachment = ColorAttachment << 1,
        SubpassInput = DepthStencilAttachment << 1
    };

    enum class BufferUsage : uint32_t
    {
        Undefined = 0,
        UniformBuffer = 1,
        StorageBuffer = 2,
        IndexBuffer = 4,
        VertexBuffer = 8,
        IndirectBuffer = 16,
        CPU
    };

    enum class DescriptorType : uint32_t
    {
        Undefined = 0,
        Sampler = 1,
        CombinedImageSampler = 2,
        SampledImage = 3,
        StorageImage = 4,
        UniformTexelBuffer = 5,
        StorageTexelBuffer = 6,
        UniformBuffer = 7,
        StorageBuffer = 8,
        UniformBufferDynamic = 9,
        StorageBufferDynamic = 10,
        InputAttachment = 11,
    };

    inline uint32_t GetAttributeSize(VertexAtributeType attributeType) {
        switch (attributeType) {
        case VertexAtributeType::int1:
        case VertexAtributeType::uint1:
        case VertexAtributeType::float1:
            return (uint32_t)sizeof(int);

        case VertexAtributeType::int2:
        case VertexAtributeType::uint2:
        case VertexAtributeType::float2:
            return (uint32_t)sizeof(int) * 2;

        case VertexAtributeType::int3:
        case VertexAtributeType::uint3:
        case VertexAtributeType::float3:
            return (uint32_t)sizeof(int) * 3;

        case VertexAtributeType::int4:
        case VertexAtributeType::uint4:
        case VertexAtributeType::float4:
            return (uint32_t)sizeof(int) * 4;

        default:
            assert(false);
            return 0;
        }
    }
}