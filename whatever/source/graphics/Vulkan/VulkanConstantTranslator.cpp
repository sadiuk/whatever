#include "VulkanConstantTranslator.h"
namespace wtv
{
    VkFormat VulkanConstantTranslator::GetVkFormatForAttribute(VertexAttributeType attributeType)
    {
        switch (attributeType) {
        case VertexAttributeType::int1:
            return VK_FORMAT_R32_SINT;
        case VertexAttributeType::int2:
            return VK_FORMAT_R32G32_SINT;
        case VertexAttributeType::int3:
            return VK_FORMAT_R32G32B32_SINT;
        case VertexAttributeType::int4:
            return VK_FORMAT_R32G32B32A32_SINT;

        case VertexAttributeType::uint1:
            return VK_FORMAT_R32_UINT;
        case VertexAttributeType::uint2:
            return VK_FORMAT_R32G32_UINT;
        case VertexAttributeType::uint3:
            return VK_FORMAT_R32G32B32_UINT;
        case VertexAttributeType::uint4:
            return VK_FORMAT_R32G32B32A32_UINT;

        case VertexAttributeType::float1:
            return VK_FORMAT_R32_SFLOAT;
        case VertexAttributeType::float2:
            return VK_FORMAT_R32G32_SFLOAT;
        case VertexAttributeType::float3:
            return VK_FORMAT_R32G32B32_SFLOAT;
        case VertexAttributeType::float4:
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

    VkDescriptorType VulkanConstantTranslator::GetVkDescriptorType(DescriptorType type)
    {
        switch (type)
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

        case DescriptorType::InlineUniformBlock:
            // Inline uniform block may be core or an EXT enum depending on header version.
#if defined(VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK)
            return VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK;
#elif defined(VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT)
            return VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT;
#else
            return VK_DESCRIPTOR_TYPE_MAX_ENUM;
#endif

        case DescriptorType::AccelerationStructureKhr:
#if defined(VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR)
            return VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
#elif defined(VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV)
            // Fallback to NV if KHR not present (numeric values differ historically).
            return VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV;
#else
            return VK_DESCRIPTOR_TYPE_MAX_ENUM;
#endif

        case DescriptorType::AccelerationStructureNv:
#if defined(VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV)
            return VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV;
#elif defined(VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR)
            return VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
#else
            return VK_DESCRIPTOR_TYPE_MAX_ENUM;
#endif

        case DescriptorType::SampleWeightImageQcom:
#if defined(VK_DESCRIPTOR_TYPE_SAMPLE_WEIGHT_IMAGE_QCOM)
            return VK_DESCRIPTOR_TYPE_SAMPLE_WEIGHT_IMAGE_QCOM;
#else
            return VK_DESCRIPTOR_TYPE_MAX_ENUM;
#endif

        case DescriptorType::BlockMatchImageQcom:
#if defined(VK_DESCRIPTOR_TYPE_BLOCK_MATCH_IMAGE_QCOM)
            return VK_DESCRIPTOR_TYPE_BLOCK_MATCH_IMAGE_QCOM;
#else
            return VK_DESCRIPTOR_TYPE_MAX_ENUM;
#endif

        case DescriptorType::MutableExt:
#if defined(VK_DESCRIPTOR_TYPE_MUTABLE_EXT)
            return VK_DESCRIPTOR_TYPE_MUTABLE_EXT;
#else
            return VK_DESCRIPTOR_TYPE_MAX_ENUM;
#endif

        case DescriptorType::Undefined:
        default:
            return VK_DESCRIPTOR_TYPE_MAX_ENUM;
        }
    }

    VkImageLayout VulkanConstantTranslator::GetVkImageLayout(ImageLayout layout)
    {
        switch (layout)
        {
        case ImageLayout::Undefined:
            return VK_IMAGE_LAYOUT_UNDEFINED;
        case ImageLayout::General:
            return VK_IMAGE_LAYOUT_GENERAL;
        case ImageLayout::ColorAttachmentOptimal:
            return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        case ImageLayout::DepthStencilAttachmentOptimal:
            return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        case ImageLayout::DepthStencilReadOnlyOptimal:
            return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
        case ImageLayout::ShaderReadOnlyOptimal:
            return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        case ImageLayout::TransferSrcOptimal:
            return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        case ImageLayout::TransferDstOptimal:
            return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        case ImageLayout::Preinitialized:
            return VK_IMAGE_LAYOUT_PREINITIALIZED;
        case ImageLayout::DepthReadOnlyStencilAttachmentOptimal:
            return VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL;
        case ImageLayout::DepthAttachmentStencilReadOnlyOptimal:
            return VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL;
        case ImageLayout::DepthAttachmentOptimal:
            return VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
        case ImageLayout::DepthReadOnlyOptimal:
            return VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL;
        case ImageLayout::StencilAttachmentOptimal:
            return VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL;
        case ImageLayout::StencilReadOnlyOptimal:
            return VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL;
        case ImageLayout::ReadOnlyOptimal:
            return VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL;
        case ImageLayout::AttachmentOptimal:
            return VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
        case ImageLayout::RenderingLocalRead:
            return VK_IMAGE_LAYOUT_RENDERING_LOCAL_READ_KHR;
        case ImageLayout::PresentSrcKhr:
            return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        case ImageLayout::VideoDecodeDstKhr:
            return VK_IMAGE_LAYOUT_VIDEO_DECODE_DST_KHR;
        case ImageLayout::VideoDecodeSrcKhr:
            return VK_IMAGE_LAYOUT_VIDEO_DECODE_SRC_KHR;
        case ImageLayout::VideoDecodeDpbKhr:
            return VK_IMAGE_LAYOUT_VIDEO_DECODE_DPB_KHR;
        case ImageLayout::SharedPresentKhr:
            return VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR;
        case ImageLayout::FragmentDensityMapOptimalExt:
            return VK_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT;
        case ImageLayout::FragmentShadingRateAttachmentOptimalKhr:
            return VK_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR;
        case ImageLayout::VideoEncodeDstKhr:
            return VK_IMAGE_LAYOUT_VIDEO_ENCODE_DST_KHR;
        case ImageLayout::VideoEncodeSrcKhr:
            return VK_IMAGE_LAYOUT_VIDEO_ENCODE_SRC_KHR;
        case ImageLayout::VideoEncodeDpbKhr:
            return VK_IMAGE_LAYOUT_VIDEO_ENCODE_DPB_KHR;
        case ImageLayout::AttachmentFeedbackLoopOptimalExt:
            return VK_IMAGE_LAYOUT_ATTACHMENT_FEEDBACK_LOOP_OPTIMAL_EXT;
        case ImageLayout::TensorAliasingArm:
            return VK_IMAGE_LAYOUT_UNDEFINED;
        case ImageLayout::VideoEncodeQuantizationMapKhr:
            return VK_IMAGE_LAYOUT_UNDEFINED;
        case ImageLayout::ZeroInitializedExt:
            return VK_IMAGE_LAYOUT_UNDEFINED;
        default:
            return VK_IMAGE_LAYOUT_UNDEFINED;
        }
    }

    VkIndexType VulkanConstantTranslator::GetVkIndexType(IndexType indexType)
    {
        switch (indexType)
        {
            case IndexType::UInt16:
				return VK_INDEX_TYPE_UINT16;
            case IndexType::UInt32:
                return VK_INDEX_TYPE_UINT32;
			case IndexType::Undefined:
				assert(false);
				return VK_INDEX_TYPE_NONE_KHR;
        }
    }

    VkImageAspectFlags VulkanConstantTranslator::GetVkImageAspectFlagBits(ImageAspectFlags aspectFlags)
    {
        VkImageAspectFlags result = 0;

        if ((aspectFlags & ImageAspectFlags::ColorBit) == ImageAspectFlags::ColorBit)           result |= VK_IMAGE_ASPECT_COLOR_BIT;
        if ((aspectFlags & ImageAspectFlags::DepthBit) == ImageAspectFlags::DepthBit)           result |= VK_IMAGE_ASPECT_DEPTH_BIT;
        if ((aspectFlags & ImageAspectFlags::StencilBit) == ImageAspectFlags::StencilBit)         result |= VK_IMAGE_ASPECT_STENCIL_BIT;
        if ((aspectFlags & ImageAspectFlags::MetadataBit) == ImageAspectFlags::MetadataBit)        result |= VK_IMAGE_ASPECT_METADATA_BIT;
        if ((aspectFlags & ImageAspectFlags::Plane0Bit) == ImageAspectFlags::Plane0Bit)          result |= VK_IMAGE_ASPECT_PLANE_0_BIT;
        if ((aspectFlags & ImageAspectFlags::Plane1Bit) == ImageAspectFlags::Plane1Bit)          result |= VK_IMAGE_ASPECT_PLANE_1_BIT;
        if ((aspectFlags & ImageAspectFlags::Plane2Bit) == ImageAspectFlags::Plane2Bit)         result |= VK_IMAGE_ASPECT_PLANE_2_BIT;
        if ((aspectFlags & ImageAspectFlags::MemoryPlane0BitExt) == ImageAspectFlags::MemoryPlane0BitExt) result |= VK_IMAGE_ASPECT_MEMORY_PLANE_0_BIT_EXT;
        if ((aspectFlags & ImageAspectFlags::MemoryPlane1BitExt) == ImageAspectFlags::MemoryPlane1BitExt) result |= VK_IMAGE_ASPECT_MEMORY_PLANE_1_BIT_EXT;
        if ((aspectFlags & ImageAspectFlags::MemoryPlane2BitExt) == ImageAspectFlags::MemoryPlane2BitExt) result |= VK_IMAGE_ASPECT_MEMORY_PLANE_2_BIT_EXT;
        if ((aspectFlags & ImageAspectFlags::MemoryPlane3BitExt) == ImageAspectFlags::MemoryPlane3BitExt) result |= VK_IMAGE_ASPECT_MEMORY_PLANE_3_BIT_EXT;

        return result;
    }

    VkMemoryPropertyFlagBits VulkanConstantTranslator::GetVkMemoryPropertyFlagBits(MemoryPropertyFlags memoryPropertyFlags)
    {
        switch (memoryPropertyFlags)
        {
            case MemoryPropertyFlags::DeviceLocal:
				return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			case MemoryPropertyFlags::HostVisible:
				return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
			case MemoryPropertyFlags::HostCoherent:
				return VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			case MemoryPropertyFlags::HostCached:
				return VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
			case MemoryPropertyFlags::LazilyAllocated:
				return VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;
			case MemoryPropertyFlags::Undefined:
				assert(false);
				return VK_MEMORY_PROPERTY_FLAG_BITS_MAX_ENUM;
        }
    }

    VkPipelineStageFlags2 VulkanConstantTranslator::GetVkPipelineStageFlags2(BarrierPipelineStageFlags stageFlags)
    {
        VkPipelineStageFlags2 result = VK_PIPELINE_STAGE_2_NONE;

        auto check = [&](BarrierPipelineStageFlags flag, VkPipelineStageFlags2 vkFlag)
            {
                if ((stageFlags & flag) == flag)
                    result |= vkFlag;
            };

        check(BarrierPipelineStageFlags::TopOfPipe, VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT);
        check(BarrierPipelineStageFlags::DrawIndirect, VK_PIPELINE_STAGE_2_DRAW_INDIRECT_BIT);
        check(BarrierPipelineStageFlags::VertexInput, VK_PIPELINE_STAGE_2_VERTEX_INPUT_BIT);
        check(BarrierPipelineStageFlags::VertexShader, VK_PIPELINE_STAGE_2_VERTEX_SHADER_BIT);
        check(BarrierPipelineStageFlags::TessellationControlShader, VK_PIPELINE_STAGE_2_TESSELLATION_CONTROL_SHADER_BIT);
        check(BarrierPipelineStageFlags::TessellationEvaluationShader, VK_PIPELINE_STAGE_2_TESSELLATION_EVALUATION_SHADER_BIT);
        check(BarrierPipelineStageFlags::GeometryShader, VK_PIPELINE_STAGE_2_GEOMETRY_SHADER_BIT);
        check(BarrierPipelineStageFlags::FragmentShader, VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT);
        check(BarrierPipelineStageFlags::EarlyFragmentTests, VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT);
        check(BarrierPipelineStageFlags::LateFragmentTests, VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT);
        check(BarrierPipelineStageFlags::ColorAttachmentOutput, VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT);
        check(BarrierPipelineStageFlags::ComputeShader, VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT);
        check(BarrierPipelineStageFlags::Transfer, VK_PIPELINE_STAGE_2_TRANSFER_BIT);
        check(BarrierPipelineStageFlags::BottomOfPipe, VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT);
        check(BarrierPipelineStageFlags::Host, VK_PIPELINE_STAGE_2_HOST_BIT);
        check(BarrierPipelineStageFlags::AllGraphics, VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT);
        check(BarrierPipelineStageFlags::AllCommands, VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT);
        check(BarrierPipelineStageFlags::Copy, VK_PIPELINE_STAGE_2_COPY_BIT);
        check(BarrierPipelineStageFlags::Resolve, VK_PIPELINE_STAGE_2_RESOLVE_BIT);
        check(BarrierPipelineStageFlags::Blit, VK_PIPELINE_STAGE_2_BLIT_BIT);
        check(BarrierPipelineStageFlags::Clear, VK_PIPELINE_STAGE_2_CLEAR_BIT);
        check(BarrierPipelineStageFlags::IndexInput, VK_PIPELINE_STAGE_2_INDEX_INPUT_BIT);
        check(BarrierPipelineStageFlags::VertexAttributeInput, VK_PIPELINE_STAGE_2_VERTEX_ATTRIBUTE_INPUT_BIT);
        check(BarrierPipelineStageFlags::PreRasterizationShaders, VK_PIPELINE_STAGE_2_PRE_RASTERIZATION_SHADERS_BIT);
        check(BarrierPipelineStageFlags::TransformFeedback, VK_PIPELINE_STAGE_2_TRANSFORM_FEEDBACK_BIT_EXT);
        check(BarrierPipelineStageFlags::ConditionalRendering, VK_PIPELINE_STAGE_2_CONDITIONAL_RENDERING_BIT_EXT);
        check(BarrierPipelineStageFlags::AccelerationStructureBuild, VK_PIPELINE_STAGE_2_ACCELERATION_STRUCTURE_BUILD_BIT_KHR);
        check(BarrierPipelineStageFlags::RayTracingShader, VK_PIPELINE_STAGE_2_RAY_TRACING_SHADER_BIT_KHR);
        check(BarrierPipelineStageFlags::FragmentShadingRate, VK_PIPELINE_STAGE_2_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR);
        check(BarrierPipelineStageFlags::TaskShader, VK_PIPELINE_STAGE_2_TASK_SHADER_BIT_EXT);
        check(BarrierPipelineStageFlags::MeshShader, VK_PIPELINE_STAGE_2_MESH_SHADER_BIT_EXT);
        check(BarrierPipelineStageFlags::FragmentDensityProcess, VK_PIPELINE_STAGE_2_FRAGMENT_DENSITY_PROCESS_BIT_EXT);
        check(BarrierPipelineStageFlags::CommandPreprocess, VK_PIPELINE_STAGE_2_COMMAND_PREPROCESS_BIT_NV);
        check(BarrierPipelineStageFlags::VideoDecode, VK_PIPELINE_STAGE_2_VIDEO_DECODE_BIT_KHR);
        check(BarrierPipelineStageFlags::VideoEncode, VK_PIPELINE_STAGE_2_VIDEO_ENCODE_BIT_KHR);
        check(BarrierPipelineStageFlags::SubpassShading, VK_PIPELINE_STAGE_2_SUBPASS_SHADER_BIT_HUAWEI);
        check(BarrierPipelineStageFlags::InvocationMask, VK_PIPELINE_STAGE_2_INVOCATION_MASK_BIT_HUAWEI);
        check(BarrierPipelineStageFlags::AccelerationStructureCopy, VK_PIPELINE_STAGE_2_ACCELERATION_STRUCTURE_COPY_BIT_KHR);
        check(BarrierPipelineStageFlags::MicromapBuild, VK_PIPELINE_STAGE_2_MICROMAP_BUILD_BIT_EXT);
        check(BarrierPipelineStageFlags::OpticalFlow, VK_PIPELINE_STAGE_2_OPTICAL_FLOW_BIT_NV);

        return result;
    }

    VkAccessFlags2 VulkanConstantTranslator::GetVkAccessFlags2(BarrierAccessFlags accessFlags)
    {
        VkAccessFlags2 result = VK_ACCESS_2_NONE;

        auto check = [&](BarrierAccessFlags flag, VkAccessFlags2 vkFlag)
            {
                if ((accessFlags & flag) == flag)
                    result |= vkFlag;
            };

        check(BarrierAccessFlags::IndirectCommandRead, VK_ACCESS_2_INDIRECT_COMMAND_READ_BIT);
        check(BarrierAccessFlags::IndexRead, VK_ACCESS_2_INDEX_READ_BIT);
        check(BarrierAccessFlags::VertexAttributeRead, VK_ACCESS_2_VERTEX_ATTRIBUTE_READ_BIT);
        check(BarrierAccessFlags::UniformRead, VK_ACCESS_2_UNIFORM_READ_BIT);
        check(BarrierAccessFlags::InputAttachmentRead, VK_ACCESS_2_INPUT_ATTACHMENT_READ_BIT);
        check(BarrierAccessFlags::ShaderRead, VK_ACCESS_2_SHADER_READ_BIT);
        check(BarrierAccessFlags::ShaderWrite, VK_ACCESS_2_SHADER_WRITE_BIT);
        check(BarrierAccessFlags::ColorAttachmentRead, VK_ACCESS_2_COLOR_ATTACHMENT_READ_BIT);
        check(BarrierAccessFlags::ColorAttachmentWrite, VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT);
        check(BarrierAccessFlags::DepthStencilAttachmentRead, VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT);
        check(BarrierAccessFlags::DepthStencilAttachmentWrite, VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
        check(BarrierAccessFlags::TransferRead, VK_ACCESS_2_TRANSFER_READ_BIT);
        check(BarrierAccessFlags::TransferWrite, VK_ACCESS_2_TRANSFER_WRITE_BIT);
        check(BarrierAccessFlags::HostRead, VK_ACCESS_2_HOST_READ_BIT);
        check(BarrierAccessFlags::HostWrite, VK_ACCESS_2_HOST_WRITE_BIT);
        check(BarrierAccessFlags::MemoryRead, VK_ACCESS_2_MEMORY_READ_BIT);
        check(BarrierAccessFlags::MemoryWrite, VK_ACCESS_2_MEMORY_WRITE_BIT);
        check(BarrierAccessFlags::CommandPreprocessRead, VK_ACCESS_2_COMMAND_PREPROCESS_READ_BIT_NV);
        check(BarrierAccessFlags::CommandPreprocessWrite, VK_ACCESS_2_COMMAND_PREPROCESS_WRITE_BIT_NV);
        check(BarrierAccessFlags::FragmentShadingRateAttachmentRead, VK_ACCESS_2_FRAGMENT_SHADING_RATE_ATTACHMENT_READ_BIT_KHR);
        check(BarrierAccessFlags::AccelerationStructureRead, VK_ACCESS_2_ACCELERATION_STRUCTURE_READ_BIT_KHR);
        check(BarrierAccessFlags::AccelerationStructureWrite, VK_ACCESS_2_ACCELERATION_STRUCTURE_WRITE_BIT_KHR);

        return result;
    }

    VkFilter VulkanConstantTranslator::GetVkFilter(SamplerFilter filter)
    {
        switch (filter)
        {
		case SamplerFilter::Nearest:
			return VK_FILTER_NEAREST;
        case SamplerFilter::Linear:
				return VK_FILTER_LINEAR;
        }
		return VK_FILTER_MAX_ENUM;
    }

    VkSamplerMipmapMode VulkanConstantTranslator::GetVkSamplerMipmapMode(SamplerMipMapMode mode)
    {
        switch (mode)
        {
		case SamplerMipMapMode::Nearest:
			return VK_SAMPLER_MIPMAP_MODE_NEAREST;
        case SamplerMipMapMode::Linear:
            return VK_SAMPLER_MIPMAP_MODE_LINEAR;
        }
        return VK_SAMPLER_MIPMAP_MODE_MAX_ENUM;
    }

    VkSamplerAddressMode VulkanConstantTranslator::GetVkSamplerAddressMode(SamplerAddressMode addressMode)
    {
        switch (addressMode)
        {
		case SamplerAddressMode::Repeat:
			return VK_SAMPLER_ADDRESS_MODE_REPEAT;
		case SamplerAddressMode::MirroredRepeat:
			return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
		case SamplerAddressMode::ClampToEdge:
			return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		case SamplerAddressMode::ClampToBorder:
			return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		case SamplerAddressMode::MirrorClampToEdge:
			return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
        }
		return VK_SAMPLER_ADDRESS_MODE_MAX_ENUM;
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
