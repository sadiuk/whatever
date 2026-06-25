#pragma once
#include "graphics/IGraphicsPipeline.h"
#include "vulkan/vulkan.h"
#include "graphics/IDevice.h"
#include "VulkanShader.h"
#include "IServiceProvider.h"
#include "graphics/IGPUResource.h"
#include "VulkanRenderPass.h"

#include <array>
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
		~VulkanGraphicsPipelineLayout();
		VkPipelineLayout GetNativeHandle() const { return m_layout; }
		IServiceProvider* GetServiceProvider() { return m_services; }
	};

	class VulkanGraphicsPipeline : public IGraphicsPipeline, public IServiceProviderHolder, public IGPUResource
	{

	public:
		VulkanGraphicsPipeline(VulkanDevice* engine, IServiceProvider* services, const CreateInfo& params, VulkanGraphicsPipelineLayout* layout);
		VkPipeline GetPipeline() { return m_pipeline; }
		~VulkanGraphicsPipeline();
		IServiceProvider* GetServiceProvider() const override;
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
		VkPipelineCache m_pipelineCache{};
		std::array<RefPtr<VulkanShader>, (size_t)ShaderStage::GraphicsStageCount> m_shaderStages;
		VulkanGraphicsPipelineLayout* m_layout;

		std::vector<VkPushConstantRange> m_pushConstantRanges;
	};
}