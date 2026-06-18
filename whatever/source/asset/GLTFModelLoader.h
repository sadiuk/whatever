#pragma once
#include "CPUMesh.h"
#include "CPUMaterial.h"
#include "util/RefPtr.h"
#include <unordered_map>

namespace wtv
{
	class GLTFModelLoader
	{
	public:
		std::vector<CPUMesh> LoadModel(const char* filePath);
	};
}