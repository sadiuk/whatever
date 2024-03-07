#pragma once
#include "ICommandBuffer.h"

#include "vulkan/vulkan.h"
#include "glm/glm.hpp"

#include <optional>
#include <vector>
#include <array>

namespace wtv
{
	class VulkanCommandBuffer : public ICommandBuffer
	{
		static constexpr uint32_t CLEAR_COLOR_SIZE = 16;
	public:
		VulkanCommandBuffer(VkDevice device, VkCommandPool commandPool);
		~VulkanCommandBuffer();
		VkCommandBuffer GetNativeHandle() { return m_commandBuffer; }
	private:
		void Begin() override;
		void End() override;
		void SetViewport(const ViewportInfo& viewport) override;
		void SetScissor(const Rect2D& scissor) override;
		void BindPipelineAndFramebuffer(IGraphicsPipeline* pipeline, IFramebuffer* framebuffer) override;
		void SetClearColorValue(uint32_t colorAttachmentIndex, void* clearColor) override;
		void SetClearDepthStencilValue(float depth, uint32_t stencil = 0) override;
		void Draw(uint32_t vertexCount, uint32_t firstVertex, uint32_t instanceCount = 0, uint32_t firstInstance = 0) override;
	private:
		std::vector<std::pair<uint32_t, std::array<std::byte, CLEAR_COLOR_SIZE>>> m_clearColorValues;
		std::optional<std::pair<float, uint32_t>> m_clearDepthStencil;
		std::optional<VkRect2D> m_scissor;
		std::optional<VkRenderPass> m_renderpass;
		VkDevice m_device;
		VkCommandBuffer m_commandBuffer;
		VkCommandPool m_commandPool;

		VkSemaphore m_queueWaitSemaphore, m_queueSignalSemaphore;
	};
}