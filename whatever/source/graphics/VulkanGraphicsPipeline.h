#pragma once
#include "IGraphicsPipeline.h"
#include "vulkan/vulkan.h"
#include "IDevice.h"
#include "VulkanShader.h"
#include "IServiceProvider.h"

#include <array>
#include "VulkanRenderPass.h"
namespace wtv
{
	class VulkanGraphicsPipelineLayout : public IGraphicsPipelineLayout
	{
		VkPipelineLayout m_layout;
		VulkanDevice* m_device;
		IServiceProvider* m_services;
		GraphicsPipelineLayoutCreateInfo m_params;
	public:
		VulkanGraphicsPipelineLayout(VulkanDevice* device, IServiceProvider* services, const GraphicsPipelineLayoutCreateInfo& params);
		VkPipelineLayout GetNativeHandle() const { return m_layout; }
		IServiceProvider* GetServiceProvider() { return m_services; }
	};

	class VulkanGraphicsPipeline : public IGraphicsPipeline, public IServiceProviderHolder
	{

	public:
		VulkanGraphicsPipeline(VulkanDevice* engine, IServiceProvider* services, const CreateInfo& params, VulkanGraphicsPipelineLayout* layout);
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
		VkPipelineCache CreatePipelineCache();

		std::vector<VkVertexInputAttributeDescription> CreateAttributeDescriptionList();
		std::vector<VkVertexInputBindingDescription> CreateBindingDescriptionList();
		std::vector<VkPushConstantRange> CreatePushConstantRanges();
	private:
		VulkanDevice* m_engine;
		IServiceProvider* m_services;
		VkPipeline m_pipeline{};
		VkDevice m_device{};
		VkPipelineCache m_pipelineCache{};
		std::array<RefPtr<VulkanShader>, (size_t)ShaderStage::GraphicsStageCount> m_shaderStages;
		VulkanGraphicsPipelineLayout* m_layout;

		std::vector<VkPushConstantRange> m_pushConstantRanges;
	};
}