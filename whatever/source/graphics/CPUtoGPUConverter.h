#pragma once
#include "asset/CPUMesh.h"
#include "graphics/GPUMesh.h"
#include "util/RefPtr.h"
#include "IServiceProvider.h"
#include "graphics/IDevice.h"

#include <unordered_map>

namespace wtv
{
	class CPUtoGPUConverter : public IReferenceCounted, IServiceProviderHolder
	{
		IServiceProvider* m_services;
		IDevice* m_device;
	public:
		using GPUMeshMap = std::unordered_map<MeshInfo::VertexLayout, std::vector<GPUMesh>, VertexLayoutHash>;
		using CPUMeshMap = std::unordered_map<MeshInfo::VertexLayout, std::vector<CPUMesh>, VertexLayoutHash>;
		CPUtoGPUConverter(IServiceProvider* services, IDevice* device) : m_services(services), m_device(device) {}
		GPUMeshMap ConvertToGPU(const CPUMeshMap& cpuMeshes);
		IServiceProvider* GetServiceProvider() const override { return m_services; }


	};
}