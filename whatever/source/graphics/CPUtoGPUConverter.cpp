#include "CPUtoGPUConverter.h"

#include "IQueue.h"

namespace wtv
{
CPUtoGPUConverter::GPUMeshMap CPUtoGPUConverter::ConvertToGPU(const CPUMeshMap& meshes)
{
    GPUMeshMap gpuMeshes;

	std::vector<uint64_t> meshVertexOffsets;
	std::vector<uint64_t> meshIndexOffsets;
	uint64_t nextVertexOffset = 0;
	uint64_t nextIndexOffset = 0;

	uint64_t lastElementVBSize = 0;
	uint64_t lastElementIBSize = 0;
	for (auto& meshVec : meshes)
	{
		for (int meshIdx = 0; meshIdx < meshVec.second.size(); meshIdx++)
		{
			meshVertexOffsets.push_back(nextVertexOffset);
			meshIndexOffsets.push_back(nextIndexOffset);
			nextVertexOffset = meshVertexOffsets.back() + meshVec.second[meshIdx].GetVertexBuffer().size();
			nextIndexOffset = meshIndexOffsets.back() + meshVec.second[meshIdx].GetIndexBuffer().size();
			lastElementVBSize = meshVec.second[meshIdx].GetVertexBuffer().size();
			lastElementIBSize = meshVec.second[meshIdx].GetIndexBuffer().size();
		}
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
	for (auto& meshVec : meshes)
	{
		for (int meshIdx = 0; meshIdx < meshVec.second.size(); meshIdx++)
		{
			memcpy((char*)mappedVertexSB + meshVertexOffsets[offsetBuffIndex], meshVec.second[meshIdx].GetVertexBuffer().data(), meshVec.second[meshIdx].GetVertexBuffer().size());
			memcpy((char*)mappedIndexSB + meshIndexOffsets[offsetBuffIndex], meshVec.second[meshIdx].GetIndexBuffer().data(), meshVec.second[meshIdx].GetIndexBuffer().size());
			offsetBuffIndex++;
		}
	}

	vertexStagingBuffer->Unmap();
	indexStagingBuffer->Unmap();

	auto vbUpdateCB = m_device->CreateCommandBuffer();
	vbUpdateCB->Begin();

	int meshIndex = 0;
	offsetBuffIndex = 0;
	for (auto& meshVec : meshes)
	{
		for (int meshIdx = 0; meshIdx < meshVec.second.size(); meshIdx++)
		{
			auto& mesh = meshVec.second[meshIdx];
			auto meshInfo = mesh.GetMeshInfo();

			auto vertexBuffer = m_device->CreateBuffer(IGPUBuffer::CreationParams{
			.bufferSize = mesh.GetVertexBuffer().size(),
			.usageFlags = BufferUsage::VertexBuffer }, "MeshVB" + std::to_string(meshIndex));

			auto indexBuffer = m_device->CreateBuffer(IGPUBuffer::CreationParams{
			.bufferSize = mesh.GetIndexBuffer().size(),
			.usageFlags = BufferUsage::IndexBuffer }, "MeshIB" + std::to_string(meshIndex));


			vbUpdateCB->CopyBuffer(vertexStagingBuffer.get(), meshVertexOffsets[meshIndex], vertexBuffer.get(), 0, mesh.GetVertexBuffer().size());
			vbUpdateCB->CopyBuffer(indexStagingBuffer.get(), meshIndexOffsets[meshIndex], indexBuffer.get(), 0, mesh.GetIndexBuffer().size());

			GPUMesh gpuMesh(meshInfo, vertexBuffer.get(), indexBuffer.get());
			gpuMeshes[meshVec.first].push_back(std::move(gpuMesh));

			offsetBuffIndex++;
			meshIndex++;
		}
	}
	vbUpdateCB->End();

	auto graphicsQueue = m_device->GetGraphicsQueue();
	QueueSubmitInfo submitInfo(vbUpdateCB.get(), m_device);
	graphicsQueue->Submit(submitInfo);
	
	return gpuMeshes;
}
}
