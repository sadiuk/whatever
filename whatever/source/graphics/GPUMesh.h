#pragma once
#include <string>
#include <array>
#include <algorithm>
#include <functional>
#include "graphics/GraphicsConstants.h"
#include "util/RefPtr.h"
#include "graphics/IGPUBuffer.h"
namespace wtv
{
	class GPUMesh
	{
		RefPtr<IGPUBuffer> m_vertexBuffer;
		RefPtr<IGPUBuffer> m_indexBuffer;
		MeshInfo m_info;

	public:
		GPUMesh(const MeshInfo& info, const RefPtr<IGPUBuffer>& vertexBuffer, const RefPtr<IGPUBuffer>& indexBuffer) : 
			m_info(info), m_vertexBuffer(vertexBuffer), m_indexBuffer(indexBuffer)
		{}

		const IGPUBuffer* GetVertexBuffer() const { return m_vertexBuffer.get(); }
		const IGPUBuffer* GetIndexBuffer() const { return m_indexBuffer.get(); }
		IGPUBuffer* GetVertexBuffer() { return m_vertexBuffer.get(); }
		IGPUBuffer* GetIndexBuffer() { return m_indexBuffer.get(); }
		const MeshInfo& GetMeshInfo() const { return m_info; }
	};
}