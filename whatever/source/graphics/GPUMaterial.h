#pragma once
#include "util/RefPtr.h"
#include "IGPUBuffer.h"
#include "IGPUImage.h"
#include "IDescriptorSet.h"

#include "glm/glm.hpp"
namespace wtv
{
	struct GPUMaterialParamEntry
	{
		glm::vec4 baseColorFactorRGBMetallicA{ 1, 1, 1, 1 };
		glm::vec4 emissiveFactorRGBroughnessA{ 0, 0, 0, 1 };
	};
	struct GPUMaterialImage
	{
		RefPtr<IGPUImage> m_texture;
		int32_t m_indexInDescriptorSet = -1;
	};
	struct GPUMaterial : public IReferenceCounted
	{
		RefPtr<IGPUBuffer> m_paramsBuffer;
		uint32_t m_indexInParamsBuffer;

		GPUMaterialImage m_diffuseTexture;
		GPUMaterialImage m_normalTexture;
		GPUMaterialImage m_metallicRoughnessTexture;
		GPUMaterialImage m_emissiveTexture;

		RefPtr<IDescriptorSet> m_descriptorSet;
	};
}