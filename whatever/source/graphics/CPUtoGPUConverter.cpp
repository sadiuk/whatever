#include "CPUtoGPUConverter.h"
#include "GPUMaterial.h"
#include "IQueue.h"

#include <map>

namespace wtv
{
CPUtoGPUConverter::GPUMeshMap CPUtoGPUConverter::ConvertToGPU(const CPUMeshVec& meshes, IDescriptorPool* descPool)
{

	GPUMeshVec gpuMeshesVec;

	auto vbUpdateCB = UploadVerticesAndIndices(meshes, gpuMeshesVec);
	RefPtr<IGPUBuffer> matStagingBuffer;
	auto materialCB = UploadMaterials(meshes, gpuMeshesVec, matStagingBuffer, descPool);

    GPUMeshMap gpuMeshes;
	for (auto mesh : gpuMeshesVec)
	{
		gpuMeshes[mesh.GetMeshInfo().vertexAttributes].push_back(mesh);
	}

	auto graphicsQueue = m_device->GetGraphicsQueue();

	ICommandBuffer* submitCBs[] = { vbUpdateCB.get(), materialCB.get() };
	QueueSubmitInfo submitInfo(submitCBs, 2, m_device->CreateBinarySemaphore());
	graphicsQueue->Submit(submitInfo);
	
	return gpuMeshes;
}

RefPtr<ICommandBuffer> CPUtoGPUConverter::UploadVerticesAndIndices(const CPUMeshVec& meshes, GPUMeshVec& gpuMeshes)
{
	std::vector<uint64_t> meshVertexOffsets;
	std::vector<uint64_t> meshIndexOffsets;
	uint64_t nextVertexOffset = 0;
	uint64_t nextIndexOffset = 0;

	uint64_t lastElementVBSize = 0;
	uint64_t lastElementIBSize = 0;
	for (int meshIdx = 0; meshIdx < meshes.size(); meshIdx++)
	{
		meshVertexOffsets.push_back(nextVertexOffset);
		meshIndexOffsets.push_back(nextIndexOffset);
		nextVertexOffset = meshVertexOffsets.back() + meshes[meshIdx].GetVertexBuffer().size();
		nextIndexOffset = meshIndexOffsets.back() + meshes[meshIdx].GetIndexBuffer().size();
		lastElementVBSize = meshes[meshIdx].GetVertexBuffer().size();
		lastElementIBSize = meshes[meshIdx].GetIndexBuffer().size();
	}

	IBuffer::CreationParams vertexStagingBufferParams{};
	vertexStagingBufferParams.bufferSize = nextVertexOffset + lastElementVBSize;
	vertexStagingBufferParams.alignment = 0;
	vertexStagingBufferParams.memoryFlags = MemoryPropertyFlags::HostVisible | MemoryPropertyFlags::HostCoherent;
	vertexStagingBufferParams.usageFlags = BufferUsage::CPU;
	RefPtr<IGPUBuffer> vertexStagingBuffer = m_device->CreateBuffer(vertexStagingBufferParams, "VertexStagingBuffer");

	IBuffer::CreationParams indexStagingBufferParams{};
	indexStagingBufferParams.bufferSize = nextIndexOffset + lastElementIBSize;
	indexStagingBufferParams.alignment = 0;
	indexStagingBufferParams.memoryFlags = MemoryPropertyFlags::HostVisible | MemoryPropertyFlags::HostCoherent;
	indexStagingBufferParams.usageFlags = BufferUsage::CPU;
	RefPtr<IGPUBuffer> indexStagingBuffer = m_device->CreateBuffer(indexStagingBufferParams, "IndexStagingBuffer");

	void* mappedVertexSB = vertexStagingBuffer->Map();
	void* mappedIndexSB = indexStagingBuffer->Map();

	int offsetBuffIndex = 0;
	for (int meshIdx = 0; meshIdx < meshes.size(); meshIdx++)
	{
		memcpy((char*)mappedVertexSB + meshVertexOffsets[offsetBuffIndex], meshes[meshIdx].GetVertexBuffer().data(), meshes[meshIdx].GetVertexBuffer().size());
		memcpy((char*)mappedIndexSB + meshIndexOffsets[offsetBuffIndex], meshes[meshIdx].GetIndexBuffer().data(), meshes[meshIdx].GetIndexBuffer().size());
		offsetBuffIndex++;
	}

	vertexStagingBuffer->Unmap();
	indexStagingBuffer->Unmap();

	auto vbUpdateCB = m_device->CreateCommandBuffer();
	vbUpdateCB->Begin();

	int meshIndex = 0;
	offsetBuffIndex = 0;
	for (int meshIdx = 0; meshIdx < meshes.size(); meshIdx++)
	{
		auto& mesh = meshes[meshIdx];
		auto meshInfo = mesh.GetMeshInfo();

		auto vertexBuffer = m_device->CreateBuffer(IGPUBuffer::CreationParams{
		.bufferSize = mesh.GetVertexBuffer().size(),
		.usageFlags = BufferUsage::VertexBuffer }, "MeshVB" + std::to_string(meshIndex));

		auto indexBuffer = m_device->CreateBuffer(IGPUBuffer::CreationParams{
		.bufferSize = mesh.GetIndexBuffer().size(),
		.usageFlags = BufferUsage::IndexBuffer }, "MeshIB" + std::to_string(meshIndex));


		vbUpdateCB->CopyBuffer(vertexStagingBuffer.get(), meshVertexOffsets[meshIndex], vertexBuffer.get(), 0, mesh.GetVertexBuffer().size());
		vbUpdateCB->CopyBuffer(indexStagingBuffer.get(), meshIndexOffsets[meshIndex], indexBuffer.get(), 0, mesh.GetIndexBuffer().size());

		gpuMeshes.emplace_back(meshInfo, vertexBuffer.get(), indexBuffer.get());

		offsetBuffIndex++;
		meshIndex++;
	}
	vbUpdateCB->End();

	return vbUpdateCB;
}

RefPtr<ICommandBuffer> CPUtoGPUConverter::UploadMaterials(const CPUMeshVec& meshes, GPUMeshVec& gpuMeshes, RefPtr<IGPUBuffer>& stagingBuffer, IDescriptorPool* descPool)
{
	std::vector<GPUMaterialParamEntry> materialParams;
	std::vector<RefPtr<GPUMaterial>> gpuMaterials;
	std::map<const CPUImageData*, GPUMaterialImage> imagesMap;

	std::map<const CPUMaterial*, RefPtr<GPUMaterial>> materialMap;
	for (int i = 0; i < meshes.size(); i++)
	{
		auto& mesh = meshes[i];
		auto& gpuMesh = gpuMeshes[i];
		if (!materialMap.contains(mesh.GetMaterial()))
		{
			materialMap[mesh.GetMaterial()] = MakeRef<GPUMaterial>();
		}
		gpuMesh.SetMaterial(materialMap[mesh.GetMaterial()]);
	}

	materialParams.reserve(materialMap.size());


	RefPtr<IGPUBuffer> materialParamsBuffer = m_device->CreateBuffer(IGPUBuffer::CreationParams{
		.bufferSize = materialMap.size() * sizeof(GPUMaterialParamEntry),
		.usageFlags = BufferUsage::StorageBuffer,
		.memoryFlags = MemoryPropertyFlags::DeviceLocal }, "MaterialParamsBuffer");

	int matIndex = 0;
	for (auto& matKey : materialMap)
	{
		auto& mat = materialParams.emplace_back();
		mat.baseColorFactorRGBMetallicA = glm::vec4(matKey.first->diffuseFactor, matKey.first->metallicFactor);
		mat.emissiveFactorRGBroughnessA = glm::vec4(matKey.first->emissiveFactor, matKey.first->roughnessFactor);

		if (matKey.first->diffuseTexture.get() && matKey.first->diffuseTexture->IsValid())
			imagesMap[matKey.first->diffuseTexture.get()] = GPUMaterialImage{ RefPtr<IGPUImage>(nullptr), -1 };
		if (matKey.first->emissiveTexture.get() && matKey.first->emissiveTexture->IsValid())
			imagesMap[matKey.first->emissiveTexture.get()] = GPUMaterialImage{ RefPtr<IGPUImage>(nullptr), -1 };
		if (matKey.first->normalTexture.get() && matKey.first->normalTexture->IsValid())
			imagesMap[matKey.first->normalTexture.get()] = GPUMaterialImage{ RefPtr<IGPUImage>(nullptr), -1 };
		if (matKey.first->metallicRoughnessTexture.get() && matKey.first->metallicRoughnessTexture->IsValid())
			imagesMap[matKey.first->metallicRoughnessTexture.get()] = GPUMaterialImage{ RefPtr<IGPUImage>(nullptr), -1 };

		//matKey.second->m_indexInParamsBuffer = matIndex++;
		matKey.second->m_paramsBuffer = materialParamsBuffer;
	}

	auto matUpdateCB = m_device->CreateCommandBuffer();
	matUpdateCB->Begin();
	
	stagingBuffer = PopulateGPUImages(matUpdateCB, imagesMap, descPool);

	DescriptorSetLayoutParams dsLayoutParams(3, true);
	dsLayoutParams.DescribeLayoutEntry(GPUMaterialDSSlot::MatParamsBufferSlot, DescriptorType::StorageBuffer, 1, ShaderStageFlags::All);
	dsLayoutParams.DescribeLayoutEntry(GPUMaterialDSSlot::MatSamplerSlot, DescriptorType::Sampler, 1, ShaderStageFlags::All);
	dsLayoutParams.DescribeLayoutEntry(GPUMaterialDSSlot::MatTexturesSlot, DescriptorType::SampledImage, imagesMap.size(), ShaderStageFlags::All);
	
	auto dsLayout = m_device->CreateDescriptorSetLayout(dsLayoutParams);
	auto descSet = descPool->AllocateDescriptorSet(dsLayout);

	
	SamplerIndex sampler = m_device->CreateSampler(SamplerCreateInfo{
		.magFilter = SamplerFilter::Linear,
		.minFilter = SamplerFilter::Linear,
		.mipmapMode = SamplerMipMapMode::Linear,
		.addressModeU = SamplerAddressMode::Repeat,
		.addressModeV = SamplerAddressMode::Repeat,
		.addressModeW = SamplerAddressMode::Repeat,
		});
	descSet->SetBinding(GPUMaterialDSSlot::MatParamsBufferSlot, materialParamsBuffer.get(), 0, materialParamsBuffer->GetSize());
	descSet->SetBinding(GPUMaterialDSSlot::MatSamplerSlot, sampler);

	int imageIndex = GPUMaterialDSSlot::MatTexturesSlot;
	for (auto& imageEntry : imagesMap)
	{
		descSet->SetBinding(imageIndex, IImage::View(imageEntry.second.m_texture.get(), ImageAspectFlags::ColorBit), ImageLayout::ShaderReadOnlyOptimal, sampler);
		imageEntry.second.m_indexInDescriptorSet = imageIndex - 2;

		imageIndex++;
	}


	matUpdateCB->UpdateBuffer(materialParamsBuffer.get(), 0, materialParamsBuffer->GetSize(), materialParams.data());


	matUpdateCB->End();

	for (auto& mat : materialMap)
	{
		auto& gpuMat = mat.second;
		if (mat.first->diffuseTexture.get() && mat.first->diffuseTexture->IsValid())
		{
			gpuMat->m_diffuseTexture = imagesMap[mat.first->diffuseTexture.get()];
		}
		if (mat.first->emissiveTexture.get() && mat.first->emissiveTexture->IsValid())
		{
			gpuMat->m_emissiveTexture = imagesMap[mat.first->emissiveTexture.get()];
		}
		if (mat.first->normalTexture.get() && mat.first->normalTexture->IsValid())
		{
			gpuMat->m_normalTexture = imagesMap[mat.first->normalTexture.get()];
		}
		if (mat.first->metallicRoughnessTexture.get() && mat.first->metallicRoughnessTexture->IsValid())
		{
			gpuMat->m_metallicRoughnessTexture = imagesMap[mat.first->metallicRoughnessTexture.get()];
		}
		gpuMat->m_descriptorSet = descSet;
	}

	


	return matUpdateCB;
}

RefPtr<IGPUBuffer> CPUtoGPUConverter::PopulateGPUImages(RefPtr<ICommandBuffer> matUpdateCB, std::map<const CPUImageData*, GPUMaterialImage>& imagesMap, IDescriptorPool* descPool)
{
	uint64_t requiredStagingBufferSize = 0;

	for (const auto& image : imagesMap)
	{
		uint64_t reqSize = image.first->CalculateRequiredStagingSize();
		requiredStagingBufferSize += reqSize;
	}
	
	IBuffer::CreationParams vertexStagingBufferParams{};
	vertexStagingBufferParams.bufferSize = requiredStagingBufferSize;
	vertexStagingBufferParams.alignment = 0;
	vertexStagingBufferParams.memoryFlags = MemoryPropertyFlags::HostVisible | MemoryPropertyFlags::HostCoherent;
	vertexStagingBufferParams.usageFlags = BufferUsage::CPU;
	RefPtr<IGPUBuffer> matTexturesStagingBuffer = m_device->CreateBuffer(vertexStagingBufferParams, "MatTexturesStagingBuffer");

	char* mappedData =(char*)matTexturesStagingBuffer->Map();
	int textureIdx = 0;

	uint64_t sourceOffset = 0;

	for (auto& image : imagesMap)
	{
		IImage::CreationParams imgCreateInfo;
		imgCreateInfo.width = image.first->width;
		imgCreateInfo.height = image.first->height;
		imgCreateInfo.depth = image.first->depth;
		imgCreateInfo.arraySize = image.first->arraySize;
		imgCreateInfo.mipLevels = image.first->mipLevels;
		imgCreateInfo.format = image.first->format;
		imgCreateInfo.dimension = image.first->dimension;
		imgCreateInfo.samples = image.first->samples;
		imgCreateInfo.usageFlags = ImageUsage::SampledImage | ImageUsage::CopyDestination;

		image.second.m_texture = m_device->CreateImage(imgCreateInfo, MemoryPropertyFlags::DeviceLocal, image.first->name + " texture" + std::to_string(sourceOffset));
		uint64_t requiredSize = image.first->CalculateRequiredStagingSize();
		memcpy(mappedData + sourceOffset, image.first->data.data(), requiredSize);

		{
			ImageBarrier beforeCopyBarrier(IImage::View(image.second.m_texture.get(), ImageAspectFlags::ColorBit));
			beforeCopyBarrier.layoutBefore = ImageLayout::Undefined;
			beforeCopyBarrier.layoutAfter = ImageLayout::TransferDstOptimal;
			beforeCopyBarrier.srcAccessMask = BarrierAccessFlags::None;
			beforeCopyBarrier.dstAccessMask = BarrierAccessFlags::TransferWrite;
			beforeCopyBarrier.srcStageMask = BarrierPipelineStageFlags::None;
			beforeCopyBarrier.dstStageMask = BarrierPipelineStageFlags::Transfer;

			matUpdateCB->PipelineBarrier(beforeCopyBarrier);
		}

		matUpdateCB->CopyBufferToImage(matTexturesStagingBuffer.get(), sourceOffset, image.second.m_texture.get(), 0, ImageAspectFlags::ColorBit);


		{
			ImageBarrier afterCopyBarrier(IImage::View(image.second.m_texture.get(), ImageAspectFlags::ColorBit));
			afterCopyBarrier.layoutBefore = ImageLayout::TransferDstOptimal;
			afterCopyBarrier.layoutAfter = ImageLayout::ShaderReadOnlyOptimal;
			afterCopyBarrier.srcAccessMask = BarrierAccessFlags::TransferWrite;
			afterCopyBarrier.dstAccessMask = BarrierAccessFlags::ShaderRead;
			afterCopyBarrier.srcStageMask = BarrierPipelineStageFlags::Transfer;
			afterCopyBarrier.dstStageMask = BarrierPipelineStageFlags::FragmentShader;

			matUpdateCB->PipelineBarrier(afterCopyBarrier);
		}
		sourceOffset += requiredSize;
	}
	matTexturesStagingBuffer->Unmap();
	return matTexturesStagingBuffer;
}

}
