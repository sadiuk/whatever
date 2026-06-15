#pragma once
#include "CPUMesh.h"
#include "util/RefPtr.h"
#include <unordered_map>

namespace wtv
{
	class GLTFModelLoader
	{
	public:
		std::unordered_map<MeshInfo::VertexLayout, std::vector<CPUMesh>, VertexLayoutHash> LoadModel(const char* filePath);
	};
}