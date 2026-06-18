#pragma once
#include "graphics/ICommandBuffer.h"
#include "VulkanSync.h"
#include "IServiceProvider.h"

#include "vulkan/vulkan.h"
#include "glm/glm.hpp"

#include <optional>
#include <vector>
#include <array>

namespace wtv
{
	class VulkanDevice;
	class VulkanGraphicsPipelineLayout;
	class VulkanGPUBuffer;
	class VulkanGPUImage;
	class VulkanCommandBuffer : public ICommandBuffer, public IServiceProviderHolder
	{
		static constexpr uint32_t CLEAR_COLOR_SIZE = 16;
	public:
		VulkanCommandBuffer(VulkanDevice* engine, VkCommandPool commandPool);
		~VulkanCommandBuffer();
		VkCommandBuffer GetNativeHandle() { return m_commandBuffer; }
	public:
		void Reset() override;
		void Begin() override;
		void End() override;
		void BeginRenderPass(IGPURenderPass* rp, IFramebuffer* framebuffer);
		void EndRenderPass();
		void SetViewport(const ViewportInfo& viewport) override;
		void SetScissor(const Rect2D& scissor) override;
		void SetPipelineLayout(const IGraphicsPipelineLayout* layout) override;
		void BindPipeline(IGraphicsPipeline* pipeline) override;
		void BindVertexBuffers(IGPUBuffer** buffers, uint32_t count, size_t* offsets) override;
		void BindIndexBuffer(IGPUBuffer* buffer, size_t offset, IndexType indexType) override;
		void SetClearColorValue(uint32_t colorAttachmentIndex, void* clearColor) override;
		void SetClearDepthStencilValue(float depth, uint32_t stencil = 0) override;
		void Draw(uint32_t vertexCount, uint32_t firstVertex, uint32_t instanceCount = 1, uint32_t firstInstance = 0) override;
		void DrawIndexed(uint32_t vertexOffset, uint32_t firstIndex, uint32_t indexCount, uint32_t instanceCount = 1, uint32_t firstInstance = 0) override;
		void UpdateBuffer(IGPUBuffer* buffer, size_t offset, size_t size, const void* data) override;
		void BindDescriptorSet(uint32_t setIndex, IDescriptorSet* set) override;
		void CopyBuffer(IGPUBuffer* src, uint64_t srcOffset, IGPUBuffer* dst, uint64_t dstOffset, uint64_t size) override;
		void CopyBufferToImage(IGPUBuffer* src, uint64_t srcOffset, IGPUImage* dst, uint32_t dstSubresource, ImageAspectFlags aspectFlags) override;
		void PipelineBarrier(const ImageBarrier& barrier) override;
		void PushConstants(const void* data, uint32_t size, ShaderStageFlags stageFlags) override;
		std::vector<IGPUResource*>& ListResourcesInUse() { return m_resourcesInUse; }
	public: 
		const VulkanFence& GetFence() const { return m_queueSignalFence; }
		IServiceProvider* GetServiceProvider() const override;
	private:
		VulkanDevice* m_engine;
		const VulkanGraphicsPipelineLayout* m_pipelineLayout;
		std::vector<std::pair<uint32_t, std::array<std::byte, CLEAR_COLOR_SIZE>>> m_clearColorValues;
		std::vector<IGPUResource*> m_resourcesInUse;
		std::optional<std::pair<float, uint32_t>> m_clearDepthStencil;
		std::optional<VkRect2D> m_scissor;
		std::optional<VkRenderPass> m_renderpass;
		VkDevice m_device;
		VkCommandBuffer m_commandBuffer;
		VkCommandPool m_commandPool;

		VulkanSemaphore m_queueWaitSemaphore, m_queueSignalSemaphore;
		VulkanFence m_queueSignalFence;
	};
}