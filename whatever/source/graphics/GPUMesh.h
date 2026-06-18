#pragma once
#include <string>
#include <array>
#include <algorithm>
#include <functional>
#include "graphics/GraphicsConstants.h"
#include "graphics/GPUMaterial.h"
#include "util/RefPtr.h"
#include "graphics/IGPUBuffer.h"

namespace wtv
{
	class GPUMesh
	{
		RefPtr<IGPUBuffer> m_vertexBuffer;
		RefPtr<IGPUBuffer> m_indexBuffer;
		MeshInfo m_info;
		RefPtr<GPUMaterial> m_material;
	public:
		GPUMesh(const MeshInfo& info, const RefPtr<IGPUBuffer>& vertexBuffer, const RefPtr<IGPUBuffer>& indexBuffer) : 
			m_info(info), m_vertexBuffer(vertexBuffer), m_indexBuffer(indexBuffer)
		{}

		const IGPUBuffer* GetVertexBuffer() const { return m_vertexBuffer.get(); }
		const IGPUBuffer* GetIndexBuffer() const { return m_indexBuffer.get(); }
		IGPUBuffer* GetVertexBuffer() { return m_vertexBuffer.get(); }
		IGPUBuffer* GetIndexBuffer() { return m_indexBuffer.get(); }
		const MeshInfo& GetMeshInfo() const { return m_info; }

		void SetMaterial(const RefPtr<GPUMaterial>& material)
		{
			m_material = material;
		}
		const GPUMaterial* GetMaterial() const { return m_material.get(); }

	};
}