#pragma once
#include "ICPUImage.h"
#include "glm/glm.hpp"

namespace wtv
{ 
struct CPUMaterial : public IReferenceCounted
{
	CPUMaterial() = default;
	CPUMaterial(const CPUMaterial& other) : 
		diffuseTexture(other.diffuseTexture),
		metallicRoughnessTexture(other.metallicRoughnessTexture),
		normalTexture(other.normalTexture),
		emissiveTexture(other.emissiveTexture),
		diffuseFactor(other.diffuseFactor),
		emissiveFactor(other.emissiveFactor),
		metallicFactor(other.metallicFactor),
		roughnessFactor(other.roughnessFactor)
	{}
	CPUMaterial& operator=(const CPUMaterial& other)
	{
		diffuseTexture = other.diffuseTexture;
		metallicRoughnessTexture = other.metallicRoughnessTexture;
		normalTexture = other.normalTexture;
		emissiveTexture = other.emissiveTexture;
		diffuseFactor = other.diffuseFactor;
		emissiveFactor = other.emissiveFactor;
		metallicFactor = other.metallicFactor;
		roughnessFactor = other.roughnessFactor;
		return *this;
	}

	float metallicFactor = 1.0f;
	float roughnessFactor = 1.0f;
	glm::vec3 emissiveFactor = glm::vec3(0.f);
	glm::vec3 diffuseFactor = glm::vec3(1.f);

	RefPtr<CPUImageData> diffuseTexture;
	RefPtr<CPUImageData> metallicRoughnessTexture;
	RefPtr<CPUImageData> normalTexture;
	RefPtr<CPUImageData> emissiveTexture;
};
}