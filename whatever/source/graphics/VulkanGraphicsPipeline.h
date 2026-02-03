#pragma once
#include "IGraphicsPipeline.h"
#include "vulkan/vulkan.h"
#include "IEngine.h"
#include "VulkanShader.h"
#include "IServiceProvider.h"

#include <array>
#include "VulkanRenderPass.h"
namespace wtv
{
	class VulkanGraphicsPipeline : public IGraphicsPipeline, public IServiceProviderHolder
	{

	public:
		VulkanGraphicsPipeline(VulkanDevice* engine, IServiceProvider* services, const CreateInfo& params);
		VkRenderPass GetRenderPass() { return m_renderPass; }
		VkPipeline GetPipeline() { return m_pipeline; }
		~VulkanGraphicsPipeline();
		IServiceProvider* GetServiceProvider() override;
	private:
		std::vector<VkPipelineShaderStageCreateInfo> CreateShaderStages();
		VkPipelineVertexInputStateCreateInfo CreatePipelineVertexInputStateCreateInfo(
			std::vector<VkVertexInputAttributeDescription>& attrDesc,
			std::vector<VkVertexInputBindingDescription>& bindingDesc
		);
		VkPipelineInputAssemblyStateCreateInfo CreatePipelineInputAssemblyStateCreateInfo();
		VkPipelineTessellationStateCreateInfo CreatePipelineTessellationStateCreateInfo();
		VkPipelineViewportStateCreateInfo CreatePipelineViewportStateCreateInfo(
			VkViewport& viewport,
			VkRect2D& scissors
		);
		VkPipelineRasterizationStateCreateInfo CreatePipelineRasterizationStateCreateInfo();
		VkPipelineMultisampleStateCreateInfo CreatePipelineMultisampleStateCreateInfo();
		VkPipelineDepthStencilStateCreateInfo CreatePipelineDepthStencilStateCreateInfo();
		VkPipelineColorBlendStateCreateInfo CreatePipelineColorBlendStateCreateInfo(
			std::vector<VkPipelineColorBlendAttachmentState>& attachmentStates
		);
		VkPipelineDynamicStateCreateInfo CreatePipelineDynamicStateCreateInfo();
		VkPipelineLayout CreatePipelineLayout();
		VkPipelineCache CreatePipelineCache();

		std::vector<VkVertexInputAttributeDescription> CreateAttributeDescriptionList();
		std::vector<VkVertexInputBindingDescription> CreateBindingDescriptionList();
		std::vector<VkPushConstantRange> CreatePushConstantRanges();
		std::vector< VkDescriptorSetLayout> CreateDescriptorSetLayouts();
	private:
		VulkanDevice* m_engine;
		IServiceProvider* m_services;
		VkPipeline m_pipeline{};
		VkDevice m_device{};
		VkRenderPass m_renderPass{};
		VkPipelineCache m_pipelineCache{};
		VkPipelineLayout m_pipelineLayout{};
		std::array<RefPtr<VulkanShader>, (size_t)ShaderStage::GraphicsStageCount> m_shaderStages;


		std::vector<VkPushConstantRange> m_pushConstantRanges;
		std::vector< VkDescriptorSetLayout> m_descSetLayouts;
	};
}