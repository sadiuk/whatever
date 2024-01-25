#pragma once
#include <cassert>
#include <cstdint>

enum class ShaderStage : uint16_t
{
	Vertex = 1,
	Geometry,
	Fragment,
    GraphicsStageCount = Fragment,


	Compute,
    Count
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

enum FrontFace 
{
    FrontFaceCounterClockwise,
    FrontFaceClockwise,
    FrontFaceUndefined
};

enum CullMode 
{
    CullModeNone,
    CullModeFront,
    CullModeBack,
    CullModeFrontAndBack,
    CullModeUndefined
};

enum PolygonMode 
{
    PolygonModeFill,
    PolygonModeLine,
    PolygonModePoint,
    PolygonModeUndefined
};

size_t GetAttributeSize(VertexAtributeType attributeType) {
    switch (attributeType) {
    case VertexAtributeType::int1:
    case VertexAtributeType::uint1:
    case VertexAtributeType::float1:
        return sizeof(int);

    case VertexAtributeType::int2:
    case VertexAtributeType::uint2:
    case VertexAtributeType::float2:
        return sizeof(int) * 2;

    case VertexAtributeType::int3:
    case VertexAtributeType::uint3:
    case VertexAtributeType::float3:
        return sizeof(int) * 3;

    case VertexAtributeType::int4:
    case VertexAtributeType::uint4:
    case VertexAtributeType::float4:
        return sizeof(int) * 4;

    default:
        assert(false);
        return 0;
    }
}