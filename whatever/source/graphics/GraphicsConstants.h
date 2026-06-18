#pragma once
#include <cassert>
#include <cstdint>
#include <climits>

#include "Windows.h"

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

	DEFINE_ENUM_FLAG_OPERATORS(ShaderStageFlags);

    enum class VertexAttributeType : uint16_t
    {
        byte1,
		byte2,
		byte3,
		byte4,
        ubyte1,
        ubyte2,
        ubyte3,
        ubyte4,
        short1,
		short2,
		short3,
		short4,
		ushort1,
		ushort2,
		ushort3,
		ushort4,
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
        Undefined
    };

    enum VertexAttributeSemantic
    {
		Undefined,
        Position,
		Normal,
		Tangent,
		Color,
		TexCoord0,
		TexCoord1,
		TexCoord2,
		TexCoord3,
		TexCoord4,
        Count

    };

    enum class IndexType
    {
        UInt16,
        UInt32,
        Undefined
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
    DEFINE_ENUM_FLAG_OPERATORS(ImageUsage);


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

    enum class MemoryPropertyFlags : uint32_t
    {
        Undefined = 0,
        DeviceLocal = 1,
        HostVisible = 2,
        HostCoherent = 4,
        HostCached = 8,
        LazilyAllocated = 16
	};
    DEFINE_ENUM_FLAG_OPERATORS(MemoryPropertyFlags);

    enum class BufferCreationFlags : uint32_t
    {
        None = 0,
        CommitedResource = 1
    };
    DEFINE_ENUM_FLAG_OPERATORS(BufferCreationFlags);

    enum class AttachmentLoadOp
    {
        Load,
        Clear,
        Undefined = -1
	};

    enum class AttachmentStoreOp
    {
        Store,
        DontCare,
        Discard,
        Undefined = -1
	};

    enum class DescriptorType
    {
        Sampler = 0,
        CombinedImageSampler = 1,
        SampledImage = 2,
        StorageImage = 3,
        UniformTexelBuffer = 4,
        StorageTexelBuffer = 5,
        UniformBuffer = 6,
        StorageBuffer = 7,
        UniformBufferDynamic = 8,
        StorageBufferDynamic = 9,
        InputAttachment = 10,
        InlineUniformBlock = 11,
        AccelerationStructureKhr = 12,
        AccelerationStructureNv = 13,
        SampleWeightImageQcom = 14,
        BlockMatchImageQcom = 15,
        MutableExt = 16,  
        Count,
        Undefined = 0x7FFFFFFF ,

    };

    enum class ImageLayout : uint32_t
    {
        Undefined = 0,
        General,
        ColorAttachmentOptimal,
        DepthStencilAttachmentOptimal,
        DepthStencilReadOnlyOptimal,
        ShaderReadOnlyOptimal,
        TransferSrcOptimal,
        TransferDstOptimal,
        Preinitialized,
        DepthReadOnlyStencilAttachmentOptimal,
        DepthAttachmentStencilReadOnlyOptimal,
        DepthAttachmentOptimal,
        DepthReadOnlyOptimal,
        StencilAttachmentOptimal,
        StencilReadOnlyOptimal,
        ReadOnlyOptimal,
        AttachmentOptimal,
        RenderingLocalRead,
        PresentSrcKhr,
        VideoDecodeDstKhr,
        VideoDecodeSrcKhr,
        VideoDecodeDpbKhr,
        SharedPresentKhr,
        FragmentDensityMapOptimalExt,
        FragmentShadingRateAttachmentOptimalKhr,
        VideoEncodeDstKhr,
        VideoEncodeSrcKhr,
        VideoEncodeDpbKhr,
        AttachmentFeedbackLoopOptimalExt,
        TensorAliasingArm,
        VideoEncodeQuantizationMapKhr,
        ZeroInitializedExt
    };

    enum class ImageAspectFlags : uint32_t
    {
        Undefined = 0,
        ColorBit = 0x00000001,
        DepthBit = 0x00000002,
        StencilBit = 0x00000004,
        MetadataBit = 0x00000008,
        Plane0Bit = 0x00000010,
        Plane1Bit = 0x00000020,
        Plane2Bit = 0x00000040,
        MemoryPlane0BitExt = 0x00000080,
        MemoryPlane1BitExt = 0x00000100,
        MemoryPlane2BitExt = 0x00000200,
        MemoryPlane3BitExt = 0x00000400,

        DepthStencilBit = 0x00000006,
    };
    DEFINE_ENUM_FLAG_OPERATORS(ImageAspectFlags);

    enum class BarrierPipelineStageFlags : uint64_t
    {
        None = 0ULL,
        TopOfPipe = 1ULL << 0,
        DrawIndirect = 1ULL << 1,
        VertexInput = 1ULL << 2,
        VertexShader = 1ULL << 3,
        TessellationControlShader = 1ULL << 4,
        TessellationEvaluationShader = 1ULL << 5,
        GeometryShader = 1ULL << 6,
        FragmentShader = 1ULL << 7,
        EarlyFragmentTests = 1ULL << 8,
        LateFragmentTests = 1ULL << 9,
        ColorAttachmentOutput = 1ULL << 10,
        ComputeShader = 1ULL << 11,
        Transfer = 1ULL << 12,
        BottomOfPipe = 1ULL << 13,
        Host = 1ULL << 14,
        AllGraphics = 1ULL << 15,
        AllCommands = 1ULL << 16,

        // Vulkan 1.3 / VK_KHR_synchronization2
        None2 = 0ULL,
        Copy = 1ULL << 32,
        Resolve = 1ULL << 33,
        Blit = 1ULL << 34,
        Clear = 1ULL << 35,
        IndexInput = 1ULL << 36,
        VertexAttributeInput = 1ULL << 37,
        PreRasterizationShaders = 1ULL << 38,

        // VK_EXT_transform_feedback
        TransformFeedback = 1ULL << 24,

        // VK_EXT_conditional_rendering
        ConditionalRendering = 1ULL << 18,

        // VK_KHR_acceleration_structure / VK_NV_ray_tracing
        AccelerationStructureBuild = 1ULL << 25,
        RayTracingShader = 1ULL << 21,

        // VK_KHR_fragment_shading_rate / VK_NV_shading_rate_image
        FragmentShadingRate = 1ULL << 22,

        // VK_EXT_mesh_shader / VK_NV_mesh_shader
        TaskShader = 1ULL << 19,
        MeshShader = 1ULL << 20,

        // VK_EXT_fragment_density_map
        FragmentDensityProcess = 1ULL << 23,

        // VK_NV_device_generated_commands
        CommandPreprocess = 1ULL << 17,

        // VK_KHR_video_decode_queue
        VideoDecode = 1ULL << 26,

        // VK_KHR_video_encode_queue
        VideoEncode = 1ULL << 27,

        // VK_HUAWEI_subpass_shading
        SubpassShading = 1ULL << 39,

        // VK_HUAWEI_invocation_mask
        InvocationMask = 1ULL << 40,

        // VK_NV_ray_tracing_motion_blur / micromap
        AccelerationStructureCopy = 1ULL << 41,
        MicromapBuild = 1ULL << 42,

        // VK_EXT_opacity_micromap
        OpticalFlow = 1ULL << 43,
    };
    DEFINE_ENUM_FLAG_OPERATORS(BarrierPipelineStageFlags);

	enum class BarrierAccessFlags : uint64_t
	{
		None = 0ULL,
		IndirectCommandRead = 1ULL << 0,
		IndexRead = 1ULL << 1,
		VertexAttributeRead = 1ULL << 2,
		UniformRead = 1ULL << 3,
		InputAttachmentRead = 1ULL << 4,
		ShaderRead = 1ULL << 5,
		ShaderWrite = 1ULL << 6,
		ColorAttachmentRead = 1ULL << 7,
		ColorAttachmentWrite = 1ULL << 8,
		DepthStencilAttachmentRead = 1ULL << 9,
		DepthStencilAttachmentWrite = 1ULL << 10,
		TransferRead = 1ULL << 11,
		TransferWrite = 1ULL << 12,
		HostRead = 1ULL << 13,
		HostWrite = 1ULL << 14,
		MemoryRead = 1ULL << 15,
		MemoryWrite = 1ULL << 16,
		// Vulkan 1.3 / VK_KHR_synchronization2
		None2 = 0ULL,
		CommandPreprocessRead = 1ULL << 17,
		CommandPreprocessWrite = 1ULL << 18,
		FragmentShadingRateAttachmentRead = 1ULL << 19,
		AccelerationStructureRead = 1ULL << 20,
		AccelerationStructureWrite = 1ULL << 21,
	};
    DEFINE_ENUM_FLAG_OPERATORS(BarrierAccessFlags);

    enum class SamplerFilter : uint32_t
    {
        undefined = 0,
		Nearest = 1,
		Linear = 2
    };

    enum class SamplerMipMapMode : uint32_t
    {
        Undefined = 0,
		Nearest = 1,
		Linear = 2
    };

    enum class SamplerAddressMode : uint32_t
    {
        Undefined = 0,
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder,
        MirrorClampToEdge
    };


    inline uint32_t GetAttributeSize(VertexAttributeType attributeType) {
        switch (attributeType) {
        case VertexAttributeType::int1:
        case VertexAttributeType::uint1:
        case VertexAttributeType::float1:
            return (uint32_t)sizeof(int);

        case VertexAttributeType::int2:
        case VertexAttributeType::uint2:
        case VertexAttributeType::float2:
            return (uint32_t)sizeof(int) * 2;

        case VertexAttributeType::int3:
        case VertexAttributeType::uint3:
        case VertexAttributeType::float3:
            return (uint32_t)sizeof(int) * 3;

        case VertexAttributeType::int4:
        case VertexAttributeType::uint4:
        case VertexAttributeType::float4:
            return (uint32_t)sizeof(int) * 4;

        default:
            assert(false);
            return 0;
        }
    }

    inline uint32_t GetIndexSize(IndexType type)
    {
        switch (type)
        {
        case IndexType::UInt16:
            return 2;
        case IndexType::UInt32:
            return 4;
        default:
            assert(false);
            return 0;
        }
    }
}