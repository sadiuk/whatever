#pragma once
#include "asset/CPUMesh.h"
#include "graphics/GPUMesh.h"
#include "util/RefPtr.h"
#include "IServiceProvider.h"
#include "graphics/IDevice.h"

#include <unordered_map>
#include <map>

namespace wtv
{
	class CPUtoGPUConverter : public IReferenceCounted, IServiceProviderHolder
	{
		IServiceProvider* m_services;
		IDevice* m_device;
	public:
		using GPUMeshMap = std::unordered_map<MeshInfo::VertexLayout, std::vector<GPUMesh>, VertexLayoutHash>;
		using CPUMeshVec = std::vector<CPUMesh>;
		using GPUMeshVec = std::vector<GPUMesh>;
		CPUtoGPUConverter(IServiceProvider* services, IDevice* device) : m_services(services), m_device(device) {}
		GPUMeshMap ConvertToGPU(const CPUMeshVec& cpuMeshes, IDescriptorPool* descPool);
		IServiceProvider* GetServiceProvider() const override { return m_services; }
	private:
		RefPtr<ICommandBuffer> UploadVerticesAndIndices(const CPUMeshVec& meshes, GPUMeshVec& gpuMeshes);
		RefPtr<ICommandBuffer> UploadMaterials(const CPUMeshVec& meshes, GPUMeshVec& gpuMeshes, RefPtr<IGPUBuffer>& stagingBuffer, IDescriptorPool* descPool);
		RefPtr<IGPUBuffer> PopulateGPUImages(RefPtr<ICommandBuffer> matUpdateCB, std::map<const CPUImageData*, GPUMaterialImage>& imagesMap, IDescriptorPool* descPool);

	};
}