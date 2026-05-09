#pragma once
#include "CPUMesh.h"
#include "util/RefPtr.h"

namespace wtv
{
	class GLTFModelLoader
	{
	public:
		std::vector<CPUMesh> LoadModel(const char* filePath);
	};
}