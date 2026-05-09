#pragma once
#include <vector>
#include <utility>
#include <string>
#include <array>
#include <algorithm>
#include "graphics/GraphicsConstants.h"

namespace wtv
{
	struct VertexAttribute
	{
		VertexAttributeType type;
		VertexAttributeSemantic semantic;
	};

	struct MeshInfo
	{
		std::vector<VertexAttribute> vertexAttributes;
		IndexType indexType;
		MeshInfo(VertexAttribute* attributes, uint32_t attributeCount, IndexType indexType) : vertexAttributes(attributeCount), indexType(indexType)
		{
			std::memcpy(vertexAttributes.data(), attributes, attributeCount * sizeof(VertexAttribute));
		}

		uint32_t GetAttributeCOunt() const { return (uint32_t)vertexAttributes.size(); }
		VertexAttribute GetAttribute(uint32_t index) const { return vertexAttributes[index]; }
	};

	class CPUMesh
	{
		std::array<std::vector<char>, VertexAttributeSemantic::Count> m_vertexBufferStaging;
		std::vector<char> m_vertexBuffer;
		std::vector<char> m_indexBuffer;
		uint32_t m_vertexCount;
		MeshInfo m_info;
	public:
		CPUMesh(const MeshInfo& meshInfo) : m_info(meshInfo)
		{
		}

		void SetVertexCount(uint32_t vertexCount)
		{
			m_vertexCount = vertexCount;
		}

		void SetVerticesAttribute(VertexAttributeSemantic attribute, const void* data, uint32_t dataSize, uint32_t dataStride, uint32_t elementSize, uint32_t elementCount)
		{
			if (dataStride == 0 || dataStride == elementSize)
			{
				m_vertexBufferStaging[attribute].resize(dataSize);
				memcpy(m_vertexBufferStaging[attribute].data(), data, dataSize);
			}
			else
			{
				m_vertexBufferStaging[attribute].resize(elementSize * elementCount);
				const char* src = static_cast<const char*>(data);
				char* dst = m_vertexBufferStaging[attribute].data();
				for (uint32_t i = 0; i < elementCount; i++)
				{
					memcpy(dst + i * elementSize, src + i * dataStride, elementSize);
				}
			}
		}

		void FormVertexBuffer()
		{
			size_t outBufferSize = 0;
			std::for_each(m_vertexBufferStaging.begin(), m_vertexBufferStaging.end(), [&outBufferSize](const std::vector<char>& vec) { outBufferSize += vec.size(); });
			m_vertexBuffer.resize(outBufferSize);
			uint64_t writeOffset = 0;
			for (int i = 0; i < m_vertexCount; i++)
			{
				for (int attrIndex = 0; attrIndex < m_info.GetAttributeCOunt(); attrIndex++)
				{
					const auto& attribute = m_info.GetAttribute(attrIndex);
					int attrSize = GetAttributeSize(attribute.type);
					memcpy(m_vertexBuffer.data() + writeOffset, m_vertexBufferStaging[attribute.semantic].data() + i * attrSize, attrSize);
					writeOffset += attrSize;
				}
			}

			std::for_each(m_vertexBufferStaging.begin(), m_vertexBufferStaging.end(), [&outBufferSize](std::vector<char>& vec) { vec.clear(); vec.shrink_to_fit(); });

		}

		void SetIndices(const void* data, uint32_t dataSize)
		{
			m_indexBuffer.resize(dataSize);
			memcpy(m_indexBuffer.data(), data, dataSize);
		}

		const std::vector<char>& GetVertexBuffer() const { return m_vertexBuffer; }
		const std::vector<char>& GetIndexBuffer() const { return m_indexBuffer; }
		const MeshInfo& GetMeshInfo() const { return m_info; }
	};
}