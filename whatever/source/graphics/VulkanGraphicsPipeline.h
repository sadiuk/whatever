#pragma once
#include "IGraphicsPipeline.h"
#include "vulkan/vulkan.hpp"
#include "IEngine.h"
#include "VulkanShader.h"
#include "IServiceProvider.h"

class VulkanGraphicsPipeline : public IGraphicsPipeline
{
	
public:
	VulkanGraphicsPipeline(IEngine* engine, IServiceProvider* services, const CreateInfo& params);
	~VulkanGraphicsPipeline();
private:
	VkPipelineCache CreatePipelineCache();
	std::vector<VkPipelineShaderStageCreateInfo> CreateShaderStages();
	VkPipelineVertexInputStateCreateInfo CreatePipelineVertexInputStateCreateInfo();
	VkPipelineInputAssemblyStateCreateInfo CreatePipelineInputAssemblyStateCreateInfo();
	VkPipelineTessellationStateCreateInfo CreatePipelineTessellationStateCreateInfo();
	VkPipelineViewportStateCreateInfo CreatePipelineViewportStateCreateInfo();
	VkPipelineRasterizationStateCreateInfo CreatePipelineRasterizationStateCreateInfo();
	VkPipelineMultisampleStateCreateInfo CreatePipelineMultisampleStateCreateInfo();
	VkPipelineDepthStencilStateCreateInfo CreatePipelineDepthStencilStateCreateInfo();
	VkPipelineColorBlendStateCreateInfo CreatePipelineColorBlendStateCreateInfo();
	VkPipelineDynamicStateCreateInfo CreatePipelineDynamicStateCreateInfo();
	VkPipelineLayout CreatePipelineLayout();
	VkRenderPass CreateRenderPass();

	std::vector<VkVertexInputAttributeDescription> CreateAttributeDescriptionList();
	std::vector<VkVertexInputBindingDescription> CreateBindingDescriptionList();
private:
	IEngine* m_engine;
	IServiceProvider* m_services;
	VkPipeline m_pipeline;
	VkDevice m_device;

	std::array<std::shared_ptr<VulkanShader>, ShaderStage::GraphicsStageCount> m_shaderStages;

};