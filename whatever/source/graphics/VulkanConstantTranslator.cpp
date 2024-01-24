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
