#include "VulkanConstantTranslator.h"
namespace wtv
{
    VkFormat VulkanConstantTranslator::GetVkFormatForAttribute(VertexAtributeType attributeType)
    {
        switch (attributeType) {
        case VertexAtributeType::int1:
            return VK_FORMAT_R32_SINT;
        case VertexAtributeType::int2:
            return VK_FORMAT_R32G32_SINT;
        case VertexAtributeType::int3:
            return VK_FORMAT_R32G32B32_SINT;
        case VertexAtributeType::int4:
            return VK_FORMAT_R32G32B32A32_SINT;

        case VertexAtributeType::uint1:
            return VK_FORMAT_R32_UINT;
        case VertexAtributeType::uint2:
            return VK_FORMAT_R32G32_UINT;
        case VertexAtributeType::uint3:
            return VK_FORMAT_R32G32B32_UINT;
        case VertexAtributeType::uint4:
            return VK_FORMAT_R32G32B32A32_UINT;

        case VertexAtributeType::float1:
            return VK_FORMAT_R32_SFLOAT;
        case VertexAtributeType::float2:
            return VK_FORMAT_R32G32_SFLOAT;
        case VertexAtributeType::float3:
            return VK_FORMAT_R32G32B32_SFLOAT;
        case VertexAtributeType::float4:
            return VK_FORMAT_R32G32B32A32_SFLOAT;

        default:
            // Handle unknown attribute type or add an assert if necessary
            return VK_FORMAT_UNDEFINED;
        }
    }

    VkShaderStageFlagBits VulkanConstantTranslator::GetVkShaderStage(ShaderStage stage)
    {
        switch (stage) {
        case ShaderStage::Vertex:
            return VK_SHADER_STAGE_VERTEX_BIT;
        case ShaderStage::Geometry:
            return VK_SHADER_STAGE_GEOMETRY_BIT;
        case ShaderStage::Fragment:
            return VK_SHADER_STAGE_FRAGMENT_BIT;
        case ShaderStage::Compute:
            return VK_SHADER_STAGE_COMPUTE_BIT;
        default:
            assert(false);
            // Handle unknown or unsupported shader stage
            return VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
        }
    }

    int VulkanConstantTranslator::GetVkShaderStageFlags(ShaderStageFlags stage)
    {
        int res = 0;
        if (stage & ShaderStageFlags::All == ShaderStageFlags::All)
            return VK_SHADER_STAGE_ALL;
        if (stage & ShaderStageFlags::Vertex)
            res |= VK_SHADER_STAGE_VERTEX_BIT;
        if (stage & ShaderStageFlags::Fragment)
            res |= VK_SHADER_STAGE_FRAGMENT_BIT;
        if (stage & ShaderStageFlags::Geometry)
            res |= VK_SHADER_STAGE_GEOMETRY_BIT;
        return res;
    }

    VkPrimitiveTopology VulkanConstantTranslator::GetVkPrimitiveTopology(PrimitiveTopology topology)
    {
        switch (topology) {
        case PrimitiveTopology::PointList:
            return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
        case PrimitiveTopology::LineList:
            return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
        case PrimitiveTopology::LineStrip:
            return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
        case PrimitiveTopology::TriangleList:
            return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        case PrimitiveTopology::TriangleStrip:
            return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
        case PrimitiveTopology::TriangleFan:
            return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
        case PrimitiveTopology::LineListWithAdjacency:
            return VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY;
        case PrimitiveTopology::LineStripWithAdjacency:
            return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY;
        case PrimitiveTopology::TriangleListWithAdjacency:
            return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY;
        case PrimitiveTopology::TriangleStripWithAdjacency:
            return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY;
        case PrimitiveTopology::PatchList:
            return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
        default:
            assert(false);
            return VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
        }
    }

    VkFrontFace VulkanConstantTranslator::GetVkFrontFace(FrontFace frontFaceEnum)
    {
        switch (frontFaceEnum) {
        case FrontFace::CounterClockwise:
            return VK_FRONT_FACE_COUNTER_CLOCKWISE;
        case FrontFace::Clockwise:
            return VK_FRONT_FACE_CLOCKWISE;
        default:
            assert(false);
            return VK_FRONT_FACE_MAX_ENUM;
        }
    }

    VkCullModeFlags VulkanConstantTranslator::GetVkCullMode(CullMode cullMode)
    {
        switch (cullMode) {
        case CullMode::None:
            return VK_CULL_MODE_NONE;
        case CullMode::Front:
            return VK_CULL_MODE_FRONT_BIT;
        case CullMode::Back:
            return VK_CULL_MODE_BACK_BIT;
        case CullMode::FrontAndBack:
            return VK_CULL_MODE_FRONT_AND_BACK;
        default:
            assert(false);
            return VK_CULL_MODE_FLAG_BITS_MAX_ENUM;
        }
    }

    VkPolygonMode VulkanConstantTranslator::GetVkPolygonMode(PolygonMode polygonModeEnum)
    {
        switch (polygonModeEnum) {
        case PolygonMode::Fill:
            return VK_POLYGON_MODE_FILL;
        case PolygonMode::Line:
            return VK_POLYGON_MODE_LINE;
        case PolygonMode::Point:
            return VK_POLYGON_MODE_POINT;
        default:
            assert(false);
            return VK_POLYGON_MODE_MAX_ENUM;
        }
    }

    VkCompareOp VulkanConstantTranslator::GetCompareOperation(CompareOperation depthTestOperationEnum)
    {
        switch (depthTestOperationEnum) {
        case CompareOperation::Never:
            return VK_COMPARE_OP_NEVER;
        case CompareOperation::Less:
            return VK_COMPARE_OP_LESS;
        case CompareOperation::Equal:
            return VK_COMPARE_OP_EQUAL;
        case CompareOperation::LessOrEqual:
            return VK_COMPARE_OP_LESS_OR_EQUAL;
        case CompareOperation::Greater:
            return VK_COMPARE_OP_GREATER;
        case CompareOperation::NotEqual:
            return VK_COMPARE_OP_NOT_EQUAL;
        case CompareOperation::GreaterOrEqual:
            return VK_COMPARE_OP_GREATER_OR_EQUAL;
        case CompareOperation::Always:
            return VK_COMPARE_OP_ALWAYS;
        default:
            assert(false);
            return VK_COMPARE_OP_MAX_ENUM;
        }
    }

    VkStencilOp VulkanConstantTranslator::GetStencilTestOperation(StencilTestOperation stencilTestOperationEnum)
    {
        switch (stencilTestOperationEnum) {
        case StencilTestOperation::Keep:
            return VK_STENCIL_OP_KEEP;
        case StencilTestOperation::Zero:
            return VK_STENCIL_OP_ZERO;
        case StencilTestOperation::Replace:
            return VK_STENCIL_OP_REPLACE;
        case StencilTestOperation::IncrementAndClamp:
            return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
        case StencilTestOperation::DecrementAndClamp:
            return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
        case StencilTestOperation::Invert:
            return VK_STENCIL_OP_INVERT;
        case StencilTestOperation::IncrementAndWrap:
            return VK_STENCIL_OP_INCREMENT_AND_WRAP;
        case StencilTestOperation::DecrementAndWrap:
            return VK_STENCIL_OP_DECREMENT_AND_WRAP;
        default:
            assert(false);
            return VK_STENCIL_OP_MAX_ENUM;
        }
    }

    VkBlendOp VulkanConstantTranslator::GetVkBlendOp(BlendOperation blendOp)
    {
        switch (blendOp) {
        case BlendOperation::Add:
            return VK_BLEND_OP_ADD;
        case BlendOperation::Subtract:
            return VK_BLEND_OP_SUBTRACT;
        case BlendOperation::ReverseSubtract:
            return VK_BLEND_OP_REVERSE_SUBTRACT;
        case BlendOperation::Min:
            return VK_BLEND_OP_MIN;
        case BlendOperation::Max:
            return VK_BLEND_OP_MAX;
        default:
            assert(false);
            return VK_BLEND_OP_MAX_ENUM;

        }
    }

    VkFormat VulkanConstantTranslator::GetVkFormat(ImageFormat imageFormat)
    {
        switch (imageFormat)
        {
        case ImageFormat::R4G4_UNORM_PACK8:
            return VK_FORMAT_R4G4_UNORM_PACK8;
        case ImageFormat::R4G4B4A4_UNORM_PACK16:
            return VK_FORMAT_R4G4B4A4_UNORM_PACK16;
        case ImageFormat::B4G4R4A4_UNORM_PACK16:
            return VK_FORMAT_B4G4R4A4_UNORM_PACK16;
        case ImageFormat::R5G6B5_UNORM_PACK16:
            return VK_FORMAT_R5G6B5_UNORM_PACK16;
        case ImageFormat::B5G6R5_UNORM_PACK16:
            return VK_FORMAT_B5G6R5_UNORM_PACK16;
        case ImageFormat::R5G5B5A1_UNORM_PACK16:
            return VK_FORMAT_R5G5B5A1_UNORM_PACK16;
        case ImageFormat::B5G5R5A1_UNORM_PACK16:
            return VK_FORMAT_B5G5R5A1_UNORM_PACK16;
        case ImageFormat::A1R5G5B5_UNORM_PACK16:
            return VK_FORMAT_A1R5G5B5_UNORM_PACK16;
        case ImageFormat::R8_UNORM:
            return VK_FORMAT_R8_UNORM;
        case ImageFormat::R8_SNORM:
            return VK_FORMAT_R8_SNORM;
        case ImageFormat::R8_USCALED:
            return VK_FORMAT_R8_USCALED;
        case ImageFormat::R8_SSCALED:
            return VK_FORMAT_R8_SSCALED;
        case ImageFormat::R8_UINT:
            return VK_FORMAT_R8_UINT;
        case ImageFormat::R8_SINT:
            return VK_FORMAT_R8_SINT;
        case ImageFormat::R8_SRGB:
            return VK_FORMAT_R8_SRGB;
        case ImageFormat::R8G8_UNORM:
            return VK_FORMAT_R8G8_UNORM;
        case ImageFormat::R8G8_SNORM:
            return VK_FORMAT_R8G8_SNORM;
        case ImageFormat::R8G8_USCALED:
            return VK_FORMAT_R8G8_USCALED;
        case ImageFormat::R8G8_SSCALED:
            return VK_FORMAT_R8G8_SSCALED;
        case ImageFormat::R8G8_UINT:
            return VK_FORMAT_R8G8_UINT;
        case ImageFormat::R8G8_SINT:
            return VK_FORMAT_R8G8_SINT;
        case ImageFormat::R8G8_SRGB:
            return VK_FORMAT_R8G8_SRGB;
        case ImageFormat::R8G8B8_UNORM:
            return VK_FORMAT_R8G8B8_UNORM;
        case ImageFormat::R8G8B8_SNORM:
            return VK_FORMAT_R8G8B8_SNORM;
        case ImageFormat::R8G8B8_USCALED:
            return VK_FORMAT_R8G8B8_USCALED;
        case ImageFormat::R8G8B8_SSCALED:
            return VK_FORMAT_R8G8B8_SSCALED;
        case ImageFormat::R8G8B8_UINT:
            return VK_FORMAT_R8G8B8_UINT;
        case ImageFormat::R8G8B8_SINT:
            return VK_FORMAT_R8G8B8_SINT;
        case ImageFormat::R8G8B8_SRGB:
            return VK_FORMAT_R8G8B8_SRGB;
        case ImageFormat::B8G8R8_UNORM:
            return VK_FORMAT_B8G8R8_UNORM;
        case ImageFormat::B8G8R8_SNORM:
            return VK_FORMAT_B8G8R8_SNORM;
        case ImageFormat::B8G8R8_USCALED:
            return VK_FORMAT_B8G8R8_USCALED;
        case ImageFormat::B8G8R8_SSCALED:
            return VK_FORMAT_B8G8R8_SSCALED;
        case ImageFormat::B8G8R8_UINT:
            return VK_FORMAT_B8G8R8_UINT;
        case ImageFormat::B8G8R8_SINT:
            return VK_FORMAT_B8G8R8_SINT;
        case ImageFormat::B8G8R8_SRGB:
            return VK_FORMAT_B8G8R8_SRGB;
        case ImageFormat::R8G8B8A8_UNORM:
            return VK_FORMAT_R8G8B8A8_UNORM;
        case ImageFormat::R8G8B8A8_SNORM:
            return VK_FORMAT_R8G8B8A8_SNORM;
        case ImageFormat::R8G8B8A8_USCALED:
            return VK_FORMAT_R8G8B8A8_USCALED;
        case ImageFormat::R8G8B8A8_SSCALED:
            return VK_FORMAT_R8G8B8A8_SSCALED;
        case ImageFormat::R8G8B8A8_UINT:
            return VK_FORMAT_R8G8B8A8_UINT;
        case ImageFormat::R8G8B8A8_SINT:
            return VK_FORMAT_R8G8B8A8_SINT;
        case ImageFormat::R8G8B8A8_SRGB:
            return VK_FORMAT_R8G8B8A8_SRGB;
        case ImageFormat::B8G8R8A8_UNORM:
            return VK_FORMAT_B8G8R8A8_UNORM;
        case ImageFormat::B8G8R8A8_SNORM:
            return VK_FORMAT_B8G8R8A8_SNORM;
        case ImageFormat::B8G8R8A8_USCALED:
            return VK_FORMAT_B8G8R8A8_USCALED;
        case ImageFormat::B8G8R8A8_SSCALED:
            return VK_FORMAT_B8G8R8A8_SSCALED;
        case ImageFormat::B8G8R8A8_UINT:
            return VK_FORMAT_B8G8R8A8_UINT;
        case ImageFormat::B8G8R8A8_SINT:
            return VK_FORMAT_B8G8R8A8_SINT;
        case ImageFormat::B8G8R8A8_SRGB:
            return VK_FORMAT_B8G8R8A8_SRGB;
        case ImageFormat::A8B8G8R8_UNORM_PACK32:
            return VK_FORMAT_A8B8G8R8_UNORM_PACK32;
        case ImageFormat::A8B8G8R8_SNORM_PACK32:
            return VK_FORMAT_A8B8G8R8_SNORM_PACK32;
        case ImageFormat::A8B8G8R8_USCALED_PACK32:
            return VK_FORMAT_A8B8G8R8_USCALED_PACK32;
        case ImageFormat::A8B8G8R8_SSCALED_PACK32:
            return VK_FORMAT_A8B8G8R8_SSCALED_PACK32;
        case ImageFormat::A8B8G8R8_UINT_PACK32:
            return VK_FORMAT_A8B8G8R8_UINT_PACK32;
        case ImageFormat::A8B8G8R8_SINT_PACK32:
            return VK_FORMAT_A8B8G8R8_SINT_PACK32;
        case ImageFormat::A8B8G8R8_SRGB_PACK32:
            return VK_FORMAT_A8B8G8R8_SRGB_PACK32;
        case ImageFormat::A2R10G10B10_UNORM_PACK32:
            return VK_FORMAT_A2R10G10B10_UNORM_PACK32;
        case ImageFormat::A2R10G10B10_SNORM_PACK32:
            return VK_FORMAT_A2R10G10B10_SNORM_PACK32;
        case ImageFormat::A2R10G10B10_USCALED_PACK32:
            return VK_FORMAT_A2R10G10B10_USCALED_PACK32;
        case ImageFormat::A2R10G10B10_SSCALED_PACK32:
            return VK_FORMAT_A2R10G10B10_SSCALED_PACK32;
        case ImageFormat::A2R10G10B10_UINT_PACK32:
            return VK_FORMAT_A2R10G10B10_UINT_PACK32;
        case ImageFormat::A2R10G10B10_SINT_PACK32:
            return VK_FORMAT_A2R10G10B10_SINT_PACK32;
        case ImageFormat::A2B10G10R10_UNORM_PACK32:
            return VK_FORMAT_A2B10G10R10_UNORM_PACK32;
        case ImageFormat::A2B10G10R10_SNORM_PACK32:
            return VK_FORMAT_A2B10G10R10_SNORM_PACK32;
        case ImageFormat::A2B10G10R10_USCALED_PACK32:
            return VK_FORMAT_A2B10G10R10_USCALED_PACK32;
        case ImageFormat::A2B10G10R10_SSCALED_PACK32:
            return VK_FORMAT_A2B10G10R10_SSCALED_PACK32;
        case ImageFormat::A2B10G10R10_UINT_PACK32:
            return VK_FORMAT_A2B10G10R10_UINT_PACK32;
        case ImageFormat::A2B10G10R10_SINT_PACK32:
            return VK_FORMAT_A2B10G10R10_SINT_PACK32;
        case ImageFormat::R16_UNORM:
            return VK_FORMAT_R16_UNORM;
        case ImageFormat::R16_SNORM:
            return VK_FORMAT_R16_SNORM;
        case ImageFormat::R16_USCALED:
            return VK_FORMAT_R16_USCALED;
        case ImageFormat::R16_SSCALED:
            return VK_FORMAT_R16_SSCALED;
        case ImageFormat::R16_UINT:
            return VK_FORMAT_R16_UINT;
        case ImageFormat::R16_SINT:
            return VK_FORMAT_R16_SINT;
        case ImageFormat::R16_SFLOAT:
            return VK_FORMAT_R16_SFLOAT;
        case ImageFormat::R16G16_UNORM:
            return VK_FORMAT_R16G16_UNORM;
        case ImageFormat::R16G16_SNORM:
            return VK_FORMAT_R16G16_SNORM;
        case ImageFormat::R16G16_USCALED:
            return VK_FORMAT_R16G16_USCALED;
        case ImageFormat::R16G16_SSCALED:
            return VK_FORMAT_R16G16_SSCALED;
        case ImageFormat::R16G16_UINT:
            return VK_FORMAT_R16G16_UINT;
        case ImageFormat::R16G16_SINT:
            return VK_FORMAT_R16G16_SINT;
        case ImageFormat::R16G16_SFLOAT:
            return VK_FORMAT_R16G16_SFLOAT;
        case ImageFormat::R16G16B16_UNORM:
            return VK_FORMAT_R16G16B16_UNORM;
        case ImageFormat::R16G16B16_SNORM:
            return VK_FORMAT_R16G16B16_SNORM;
        case ImageFormat::R16G16B16_USCALED:
            return VK_FORMAT_R16G16B16_USCALED;
        case ImageFormat::R16G16B16_SSCALED:
            return VK_FORMAT_R16G16B16_SSCALED;
        case ImageFormat::R16G16B16_UINT:
            return VK_FORMAT_R16G16B16_UINT;
        case ImageFormat::R16G16B16_SINT:
            return VK_FORMAT_R16G16B16_SINT;
        case ImageFormat::R16G16B16_SFLOAT:
            return VK_FORMAT_R16G16B16_SFLOAT;
        case ImageFormat::R16G16B16A16_UNORM:
            return VK_FORMAT_R16G16B16A16_UNORM;
        case ImageFormat::R16G16B16A16_SNORM:
            return VK_FORMAT_R16G16B16A16_SNORM;
        case ImageFormat::R16G16B16A16_USCALED:
            return VK_FORMAT_R16G16B16A16_USCALED;
        case ImageFormat::R16G16B16A16_SSCALED:
            return VK_FORMAT_R16G16B16A16_SSCALED;
        case ImageFormat::R16G16B16A16_UINT:
            return VK_FORMAT_R16G16B16A16_UINT;
        case ImageFormat::R16G16B16A16_SINT:
            return VK_FORMAT_R16G16B16A16_SINT;
        case ImageFormat::R16G16B16A16_SFLOAT:
            return VK_FORMAT_R16G16B16A16_SFLOAT;
        case ImageFormat::R32_UINT:
            return VK_FORMAT_R32_UINT;
        case ImageFormat::R32_SINT:
            return VK_FORMAT_R32_SINT;
        case ImageFormat::R32_SFLOAT:
            return VK_FORMAT_R32_SFLOAT;
        case ImageFormat::R32G32_UINT:
            return VK_FORMAT_R32G32_UINT;
        case ImageFormat::R32G32_SINT:
            return VK_FORMAT_R32G32_SINT;
        case ImageFormat::R32G32_SFLOAT:
            return VK_FORMAT_R32G32_SFLOAT;
        case ImageFormat::R32G32B32_UINT:
            return VK_FORMAT_R32G32B32_UINT;
        case ImageFormat::R32G32B32_SINT:
            return VK_FORMAT_R32G32B32_SINT;
        case ImageFormat::R32G32B32_SFLOAT:
            return VK_FORMAT_R32G32B32_SFLOAT;
        case ImageFormat::R32G32B32A32_UINT:
            return VK_FORMAT_R32G32B32A32_UINT;
        case ImageFormat::R32G32B32A32_SINT:
            return VK_FORMAT_R32G32B32A32_SINT;
        case ImageFormat::R32G32B32A32_SFLOAT:
            return VK_FORMAT_R32G32B32A32_SFLOAT;
        case ImageFormat::D16_UNORM:
            return VK_FORMAT_D16_UNORM;
        case ImageFormat::D32_SFLOAT:
            return VK_FORMAT_D32_SFLOAT;
        case ImageFormat::S8_UINT:
            return VK_FORMAT_S8_UINT;
        case ImageFormat::D16_UNORM_S8_UINT:
            return VK_FORMAT_D16_UNORM_S8_UINT;
        case ImageFormat::D24_UNORM_S8_UINT:
            return VK_FORMAT_D24_UNORM_S8_UINT;
        case ImageFormat::D32_SFLOAT_S8_UINT:
            return VK_FORMAT_D32_SFLOAT_S8_UINT;
        case ImageFormat::Undefined:
        default:
            return VK_FORMAT_UNDEFINED;
        }
    }

    VkImageType VulkanConstantTranslator::GetVkImageType(ImageDimension dim)
    {
        switch (dim)
        {
        case ImageDimension::Dimension1D:
                return VK_IMAGE_TYPE_1D;
        case ImageDimension::Dimension2D:
            return VK_IMAGE_TYPE_2D;
        case ImageDimension::Dimension3D:
            return VK_IMAGE_TYPE_3D;
        default:
            assert(false);
            return VK_IMAGE_TYPE_MAX_ENUM;
        }
    }

    ImageFormat VulkanConstantTranslator::GetEngineImageFormat(VkFormat imageFormat)
    {
        switch (imageFormat)
        {
        case VK_FORMAT_R4G4_UNORM_PACK8:
            return ImageFormat::R4G4_UNORM_PACK8;
        case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
            return ImageFormat::R4G4B4A4_UNORM_PACK16;
        case VK_FORMAT_B4G4R4A4_UNORM_PACK16:
            return ImageFormat::B4G4R4A4_UNORM_PACK16;
        case VK_FORMAT_R5G6B5_UNORM_PACK16:
            return ImageFormat::R5G6B5_UNORM_PACK16;
        case VK_FORMAT_B5G6R5_UNORM_PACK16:
            return ImageFormat::B5G6R5_UNORM_PACK16;
        case VK_FORMAT_R5G5B5A1_UNORM_PACK16:
            return ImageFormat::R5G5B5A1_UNORM_PACK16;
        case VK_FORMAT_B5G5R5A1_UNORM_PACK16:
            return ImageFormat::B5G5R5A1_UNORM_PACK16;
        case VK_FORMAT_A1R5G5B5_UNORM_PACK16:
            return ImageFormat::A1R5G5B5_UNORM_PACK16;
        case VK_FORMAT_R8_UNORM:
            return ImageFormat::R8_UNORM;
        case VK_FORMAT_R8_SNORM:
            return ImageFormat::R8_SNORM;
        case VK_FORMAT_R8_USCALED:
            return ImageFormat::R8_USCALED;
        case VK_FORMAT_R8_SSCALED:
            return ImageFormat::R8_SSCALED;
        case VK_FORMAT_R8_UINT:
            return ImageFormat::R8_UINT;
        case VK_FORMAT_R8_SINT:
            return ImageFormat::R8_SINT;
        case VK_FORMAT_R8_SRGB:
            return ImageFormat::R8_SRGB;
        case VK_FORMAT_R8G8_UNORM:
            return ImageFormat::R8G8_UNORM;
        case VK_FORMAT_R8G8_SNORM:
            return ImageFormat::R8G8_SNORM;
        case VK_FORMAT_R8G8_USCALED:
            return ImageFormat::R8G8_USCALED;
        case VK_FORMAT_R8G8_SSCALED:
            return ImageFormat::R8G8_SSCALED;
        case VK_FORMAT_R8G8_UINT:
            return ImageFormat::R8G8_UINT;
        case VK_FORMAT_R8G8_SINT:
            return ImageFormat::R8G8_SINT;
        case VK_FORMAT_R8G8_SRGB:
            return ImageFormat::R8G8_SRGB;
        case VK_FORMAT_R8G8B8_UNORM:
            return ImageFormat::R8G8B8_UNORM;
        case VK_FORMAT_R8G8B8_SNORM:
            return ImageFormat::R8G8B8_SNORM;
        case VK_FORMAT_R8G8B8_USCALED:
            return ImageFormat::R8G8B8_USCALED;
        case VK_FORMAT_R8G8B8_SSCALED:
            return ImageFormat::R8G8B8_SSCALED;
        case VK_FORMAT_R8G8B8_UINT:
            return ImageFormat::R8G8B8_UINT;
        case VK_FORMAT_R8G8B8_SINT:
            return ImageFormat::R8G8B8_SINT;
        case VK_FORMAT_R8G8B8_SRGB:
            return ImageFormat::R8G8B8_SRGB;
        case VK_FORMAT_B8G8R8_UNORM:
            return ImageFormat::B8G8R8_UNORM;
        case VK_FORMAT_B8G8R8_SNORM:
            return ImageFormat::B8G8R8_SNORM;
        case VK_FORMAT_B8G8R8_USCALED:
            return ImageFormat::B8G8R8_USCALED;
        case VK_FORMAT_B8G8R8_SSCALED:
            return ImageFormat::B8G8R8_SSCALED;
        case VK_FORMAT_B8G8R8_UINT:
            return ImageFormat::B8G8R8_UINT;
        case VK_FORMAT_B8G8R8_SINT:
            return ImageFormat::B8G8R8_SINT;
        case VK_FORMAT_B8G8R8_SRGB:
            return ImageFormat::B8G8R8_SRGB;
        case VK_FORMAT_R8G8B8A8_UNORM:
            return ImageFormat::R8G8B8A8_UNORM;
        case VK_FORMAT_R8G8B8A8_SNORM:
            return ImageFormat::R8G8B8A8_SNORM;
        case VK_FORMAT_R8G8B8A8_USCALED:
            return ImageFormat::R8G8B8A8_USCALED;
        case VK_FORMAT_R8G8B8A8_SSCALED:
            return ImageFormat::R8G8B8A8_SSCALED;
        case VK_FORMAT_R8G8B8A8_UINT:
            return ImageFormat::R8G8B8A8_UINT;
        case VK_FORMAT_R8G8B8A8_SINT:
            return ImageFormat::R8G8B8A8_SINT;
        case VK_FORMAT_R8G8B8A8_SRGB:
            return ImageFormat::R8G8B8A8_SRGB;
        case VK_FORMAT_B8G8R8A8_UNORM:
            return ImageFormat::B8G8R8A8_UNORM;
        case VK_FORMAT_B8G8R8A8_SNORM:
            return ImageFormat::B8G8R8A8_SNORM;
        case VK_FORMAT_B8G8R8A8_USCALED:
            return ImageFormat::B8G8R8A8_USCALED;
        case VK_FORMAT_B8G8R8A8_SSCALED:
            return ImageFormat::B8G8R8A8_SSCALED;
        case VK_FORMAT_B8G8R8A8_UINT:
            return ImageFormat::B8G8R8A8_UINT;
        case VK_FORMAT_B8G8R8A8_SINT:
            return ImageFormat::B8G8R8A8_SINT;
        case VK_FORMAT_B8G8R8A8_SRGB:
            return ImageFormat::B8G8R8A8_SRGB;
        case VK_FORMAT_A8B8G8R8_UNORM_PACK32:
            return ImageFormat::A8B8G8R8_UNORM_PACK32;
        case VK_FORMAT_A8B8G8R8_SNORM_PACK32:
            return ImageFormat::A8B8G8R8_SNORM_PACK32;
        case VK_FORMAT_A8B8G8R8_USCALED_PACK32:
            return ImageFormat::A8B8G8R8_USCALED_PACK32;
        case VK_FORMAT_A8B8G8R8_SSCALED_PACK32:
            return ImageFormat::A8B8G8R8_SSCALED_PACK32;
        case VK_FORMAT_A8B8G8R8_UINT_PACK32:
            return ImageFormat::A8B8G8R8_UINT_PACK32;
        case VK_FORMAT_A8B8G8R8_SINT_PACK32:
            return ImageFormat::A8B8G8R8_SINT_PACK32;
        case VK_FORMAT_A8B8G8R8_SRGB_PACK32:
            return ImageFormat::A8B8G8R8_SRGB_PACK32;
        case VK_FORMAT_A2R10G10B10_UNORM_PACK32:
            return ImageFormat::A2R10G10B10_UNORM_PACK32;
        case VK_FORMAT_A2R10G10B10_SNORM_PACK32:
            return ImageFormat::A2R10G10B10_SNORM_PACK32;
        case VK_FORMAT_A2R10G10B10_USCALED_PACK32:
            return ImageFormat::A2R10G10B10_USCALED_PACK32;
        case VK_FORMAT_A2R10G10B10_SSCALED_PACK32:
            return ImageFormat::A2R10G10B10_SSCALED_PACK32;
        case VK_FORMAT_A2R10G10B10_UINT_PACK32:
            return ImageFormat::A2R10G10B10_UINT_PACK32;
        case VK_FORMAT_A2R10G10B10_SINT_PACK32:
            return ImageFormat::A2R10G10B10_SINT_PACK32;
        case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
            return ImageFormat::A2B10G10R10_UNORM_PACK32;
        case VK_FORMAT_A2B10G10R10_SNORM_PACK32:
            return ImageFormat::A2B10G10R10_SNORM_PACK32;
        case VK_FORMAT_A2B10G10R10_USCALED_PACK32:
            return ImageFormat::A2B10G10R10_USCALED_PACK32;
        case VK_FORMAT_A2B10G10R10_SSCALED_PACK32:
            return ImageFormat::A2B10G10R10_SSCALED_PACK32;
        case VK_FORMAT_A2B10G10R10_UINT_PACK32:
            return ImageFormat::A2B10G10R10_UINT_PACK32;
        case VK_FORMAT_A2B10G10R10_SINT_PACK32:
            return ImageFormat::A2B10G10R10_SINT_PACK32;
        case VK_FORMAT_R16_UNORM:
            return ImageFormat::R16_UNORM;
        case VK_FORMAT_R16_SNORM:
            return ImageFormat::R16_SNORM;
        case VK_FORMAT_R16_USCALED:
            return ImageFormat::R16_USCALED;
        case VK_FORMAT_R16_SSCALED:
            return ImageFormat::R16_SSCALED;
        case VK_FORMAT_R16_UINT:
            return ImageFormat::R16_UINT;
        case VK_FORMAT_R16_SINT:
            return ImageFormat::R16_SINT;
        case VK_FORMAT_R16_SFLOAT:
            return ImageFormat::R16_SFLOAT;
        case VK_FORMAT_R16G16_UNORM:
            return ImageFormat::R16G16_UNORM;
        case VK_FORMAT_R16G16_SNORM:
            return ImageFormat::R16G16_SNORM;
        case VK_FORMAT_R16G16_USCALED:
            return ImageFormat::R16G16_USCALED;
        case VK_FORMAT_R16G16_SSCALED:
            return ImageFormat::R16G16_SSCALED;
        case VK_FORMAT_R16G16_UINT:
            return ImageFormat::R16G16_UINT;
        case VK_FORMAT_R16G16_SINT:
            return ImageFormat::R16G16_SINT;
        case VK_FORMAT_R16G16_SFLOAT:
            return ImageFormat::R16G16_SFLOAT;
        case VK_FORMAT_R16G16B16_UNORM:
            return ImageFormat::R16G16B16_UNORM;
        case VK_FORMAT_R16G16B16_SNORM:
            return ImageFormat::R16G16B16_SNORM;
        case VK_FORMAT_R16G16B16_USCALED:
            return ImageFormat::R16G16B16_USCALED;
        case VK_FORMAT_R16G16B16_SSCALED:
            return ImageFormat::R16G16B16_SSCALED;
        case VK_FORMAT_R16G16B16_UINT:
            return ImageFormat::R16G16B16_UINT;
        case VK_FORMAT_R16G16B16_SINT:
            return ImageFormat::R16G16B16_SINT;
        case VK_FORMAT_R16G16B16_SFLOAT:
            return ImageFormat::R16G16B16_SFLOAT;
        case VK_FORMAT_R16G16B16A16_UNORM:
            return ImageFormat::R16G16B16A16_UNORM;
        case VK_FORMAT_R16G16B16A16_SNORM:
            return ImageFormat::R16G16B16A16_SNORM;
        case VK_FORMAT_R16G16B16A16_USCALED:
            return ImageFormat::R16G16B16A16_USCALED;
        case VK_FORMAT_R16G16B16A16_SSCALED:
            return ImageFormat::R16G16B16A16_SSCALED;
        case VK_FORMAT_R16G16B16A16_UINT:
            return ImageFormat::R16G16B16A16_UINT;
        case VK_FORMAT_R16G16B16A16_SINT:
            return ImageFormat::R16G16B16A16_SINT;
        case VK_FORMAT_R16G16B16A16_SFLOAT:
            return ImageFormat::R16G16B16A16_SFLOAT;
        case VK_FORMAT_R32_UINT:
            return ImageFormat::R32_UINT;
        case VK_FORMAT_R32_SINT:
            return ImageFormat::R32_SINT;
        case VK_FORMAT_R32_SFLOAT:
            return ImageFormat::R32_SFLOAT;
        case VK_FORMAT_R32G32_UINT:
            return ImageFormat::R32G32_UINT;
        case VK_FORMAT_R32G32_SINT:
            return ImageFormat::R32G32_SINT;
        case VK_FORMAT_R32G32_SFLOAT:
            return ImageFormat::R32G32_SFLOAT;
        case VK_FORMAT_R32G32B32_UINT:
            return ImageFormat::R32G32B32_UINT;
        case VK_FORMAT_R32G32B32_SINT:
            return ImageFormat::R32G32B32_SINT;
        case VK_FORMAT_R32G32B32_SFLOAT:
            return ImageFormat::R32G32B32_SFLOAT;
        case VK_FORMAT_R32G32B32A32_UINT:
            return ImageFormat::R32G32B32A32_UINT;
        case VK_FORMAT_R32G32B32A32_SINT:
            return ImageFormat::R32G32B32A32_SINT;
        case VK_FORMAT_R32G32B32A32_SFLOAT:
            return ImageFormat::R32G32B32A32_SFLOAT;
        case VK_FORMAT_D16_UNORM:
            return ImageFormat::D16_UNORM;
        case VK_FORMAT_D32_SFLOAT:
            return ImageFormat::D32_SFLOAT;
        case VK_FORMAT_S8_UINT:
            return ImageFormat::S8_UINT;
        case VK_FORMAT_D16_UNORM_S8_UINT:
            return ImageFormat::D16_UNORM_S8_UINT;
        case VK_FORMAT_D24_UNORM_S8_UINT:
            return ImageFormat::D24_UNORM_S8_UINT;
        case VK_FORMAT_D32_SFLOAT_S8_UINT:
            return ImageFormat::D32_SFLOAT_S8_UINT;
        case VK_FORMAT_UNDEFINED:
            return ImageFormat::Undefined;
        default:
            return ImageFormat::Undefined;
        }
    }

    uint32_t VulkanConstantTranslator::GetVkImageUsageMask(std::underlying_type_t<ImageUsage> usageBits)
    {
        if (usageBits == (uint32_t)ImageUsage::Undefined)
            return VK_IMAGE_USAGE_FLAG_BITS_MAX_ENUM;

        uint32_t result = 0;

        if (usageBits & (uint32_t)ImageUsage::CopySource)
            result |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        if (usageBits & (uint32_t)ImageUsage::CopyDestination)
            result |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        if (usageBits & (uint32_t)ImageUsage::SampledImage)
            result |= VK_IMAGE_USAGE_SAMPLED_BIT;
        if (usageBits & (uint32_t)ImageUsage::StorageImage)
            result |= VK_IMAGE_USAGE_STORAGE_BIT;
        if (usageBits & (uint32_t)ImageUsage::ColorAttachment)
            result |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        if (usageBits & (uint32_t)ImageUsage::DepthStencilAttachment)
            result |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        if (usageBits & (uint32_t)ImageUsage::SubpassInput)
            result |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
        return result;
    }

    VkBufferUsageFlags VulkanConstantTranslator::GetVkBufferUsageFlags(std::underlying_type_t<BufferUsage> usageBits)
    {
        VkBufferUsageFlags result = 0;
        if (usageBits & (uint32_t)BufferUsage::UniformBuffer)
            result |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        if(usageBits & (uint32_t)BufferUsage::StorageBuffer)
            result |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
        if(usageBits & (uint32_t)BufferUsage::IndexBuffer)
            result |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        if(usageBits & (uint32_t)BufferUsage::VertexBuffer)
            result |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        if(usageBits & (uint32_t)BufferUsage::IndirectBuffer)
            result |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
        return result;
    }

    VkDescriptorType VulkanConstantTranslator::GetVKDescriptorType(DescriptorType dsType)
    {
        switch (dsType)
        {
        case DescriptorType::Sampler:
            return VK_DESCRIPTOR_TYPE_SAMPLER;
        case DescriptorType::CombinedImageSampler:
            return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        case DescriptorType::SampledImage:
            return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
        case DescriptorType::StorageImage:
            return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        case DescriptorType::UniformTexelBuffer:
            return VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
        case DescriptorType::StorageTexelBuffer:
            return VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
        case DescriptorType::UniformBuffer:
            return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        case DescriptorType::StorageBuffer:
            return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        case DescriptorType::UniformBufferDynamic:
            return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        case DescriptorType::StorageBufferDynamic:
            return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
        case DescriptorType::InputAttachment:
            return VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
        case DescriptorType::Undefined:
        default:
            assert(false);
            return VK_DESCRIPTOR_TYPE_MAX_ENUM; // Undefined or invalid type
        }
    }

    VkAttachmentStoreOp VulkanConstantTranslator::GetVkAttachmentStoreOp(AttachmentStoreOp storeOp)
    {
        switch (storeOp)
        {
            case AttachmentStoreOp::Store:
				return VK_ATTACHMENT_STORE_OP_STORE;
			case AttachmentStoreOp::DontCare:
                return VK_ATTACHMENT_STORE_OP_DONT_CARE;
			case AttachmentStoreOp::Discard:
				return VK_ATTACHMENT_STORE_OP_NONE;
        }
        assert(false);
		return VK_ATTACHMENT_STORE_OP_MAX_ENUM;
    }

    VkAttachmentLoadOp VulkanConstantTranslator::GetVkAttachmentLoadOp(AttachmentLoadOp loadOp)
    {
        switch (loadOp)
        {
            case AttachmentLoadOp::Load:
			    return VK_ATTACHMENT_LOAD_OP_LOAD;
            case AttachmentLoadOp::Clear:
				return VK_ATTACHMENT_LOAD_OP_CLEAR;
        }
        assert(false);
		return VK_ATTACHMENT_LOAD_OP_MAX_ENUM;
    }

    VkBlendFactor VulkanConstantTranslator::GetVkBlendFactor(BlendFactor blendFactor)
    {
        switch (blendFactor) {
        case BlendFactor::Zero:
            return VK_BLEND_FACTOR_ZERO;
        case BlendFactor::One:
            return VK_BLEND_FACTOR_ONE;
        case BlendFactor::SrcColor:
            return VK_BLEND_FACTOR_SRC_COLOR;
        case BlendFactor::OneMinusSrcColor:
            return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
        case BlendFactor::DstColor:
            return VK_BLEND_FACTOR_DST_COLOR;
        case BlendFactor::OneMinusDstColor:
            return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
        case BlendFactor::SrcAlpha:
            return VK_BLEND_FACTOR_SRC_ALPHA;
        case BlendFactor::OneMinusSrcAlpha:
            return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        case BlendFactor::DstAlpha:
            return VK_BLEND_FACTOR_DST_ALPHA;
        case BlendFactor::OneMinusDstAlpha:
            return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
        case BlendFactor::ConstantColor:
            return VK_BLEND_FACTOR_CONSTANT_COLOR;
        case BlendFactor::OneMinusConstantColor:
            return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
        case BlendFactor::ConstantAlpha:
            return VK_BLEND_FACTOR_CONSTANT_ALPHA;
        case BlendFactor::OneMinusConstantAlpha:
            return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
        case BlendFactor::SrcAlphaSaturate:
            return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
        case BlendFactor::Src1Color:
            return VK_BLEND_FACTOR_SRC1_COLOR;
        case BlendFactor::OneMinusSrc1Color:
            return VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
        case BlendFactor::Src1Alpha:
            return VK_BLEND_FACTOR_SRC1_ALPHA;
        case BlendFactor::OneMinusSrc1Alpha:
            return VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
        case BlendFactor::Undefined:
            assert(false);
            return VK_BLEND_FACTOR_MAX_ENUM;
        }
        return VK_BLEND_FACTOR_MAX_ENUM;

    }
}
