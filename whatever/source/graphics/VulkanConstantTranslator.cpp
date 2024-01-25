#include "VulkanConstantTranslator.h"

VkFormat VulkanConstantsTranslator::GetVkFormatForAttribute(VertexAtributeType attributeType)
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

VkShaderStageFlagBits VulkanConstantsTranslator::GetVkShaderStage(ShaderStage stage)
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

VkPrimitiveTopology VulkanConstantsTranslator::GetVkPrimitiveTopology(PrimitiveTopology topology)
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
        // Handle unknown or unsupported topology
        return VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
    }
}

VkFrontFace VulkanConstantsTranslator::GetVkFrontFace(FrontFace frontFaceEnum)
{
    switch (frontFaceEnum) {
    case FrontFaceCounterClockwise:
        return VK_FRONT_FACE_COUNTER_CLOCKWISE;
    case FrontFaceClockwise:
        return VK_FRONT_FACE_CLOCKWISE;
    default:
        assert(false);
        return VK_FRONT_FACE_MAX_ENUM;
    }
}

VkCullModeFlags VulkanConstantsTranslator::GetVkCullMode(CullMode cullMode)
{
    switch (cullMode) {
    case CullModeNone:
        return VK_CULL_MODE_NONE;
    case CullModeFront:
        return VK_CULL_MODE_FRONT_BIT;
    case CullModeBack:
        return VK_CULL_MODE_BACK_BIT;
    case CullModeFrontAndBack:
        return VK_CULL_MODE_FRONT_AND_BACK;
    default:
        assert(false);
        return VK_CULL_MODE_FLAG_BITS_MAX_ENUM;
    }
}

VkPolygonMode VulkanConstantsTranslator::GetVkPolygonMode(PolygonMode polygonModeEnum)
{
    switch (polygonModeEnum) {
    case PolygonModeFill:
        return VK_POLYGON_MODE_FILL;
    case PolygonModeLine:
        return VK_POLYGON_MODE_LINE;
    case PolygonModePoint:
        return VK_POLYGON_MODE_POINT;
    default:
        assert(false);
        return VK_POLYGON_MODE_MAX_ENUM;
    }
}
