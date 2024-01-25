#include "VulkanGraphicsPipeline.h"
#include "VulkanEngine.h"
#include "VkMakros.h"
#include "VulkanConstantTranslator.h"
#include "IVulkanShaderCompiler.h"

VulkanGraphicsPipeline::VulkanGraphicsPipeline(IEngine* engine, IServiceProvider* services, const CreateInfo& params) :
	IGraphicsPipeline(params),
	m_engine(engine),
	m_services(services)
{
	m_device = static_cast<VulkanEngine*>(m_engine)->GetNativeDeviceHandle();

	VkPipelineCache pipelineCache = CreatePipelineCache();
	std::vector<VkPipelineShaderStageCreateInfo> shaderStages = CreateShaderStages();
	VkPipelineVertexInputStateCreateInfo verterInputStateInfo = CreatePipelineVertexInputStateCreateInfo();
	VkPipelineInputAssemblyStateCreateInfo assInfo = CreatePipelineInputAssemblyStateCreateInfo(); // Totally intentional
	VkPipelineTessellationStateCreateInfo tessInfo = CreatePipelineTessellationStateCreateInfo();
	VkPipelineViewportStateCreateInfo viewportInfo = CreatePipelineViewportStateCreateInfo();
	VkPipelineRasterizationStateCreateInfo rasterStateInfo = CreatePipelineRasterizationStateCreateInfo();
	VkPipelineMultisampleStateCreateInfo msInfo = CreatePipelineMultisampleStateCreateInfo();
	VkPipelineDepthStencilStateCreateInfo dsInfo = CreatePipelineDepthStencilStateCreateInfo();
	VkPipelineColorBlendStateCreateInfo blendStateInfo = CreatePipelineColorBlendStateCreateInfo();
	VkPipelineDynamicStateCreateInfo dynamicStateInfo = CreatePipelineDynamicStateCreateInfo();
	VkPipelineLayout pipelineLayout = CreatePipelineLayout();
	VkRenderPass renderPass = CreateRenderPass();

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.pNext = nullptr;
	pipelineInfo.flags = 0;
	pipelineInfo.stageCount = shaderStages.size();
	pipelineInfo.pStages = shaderStages.data();
	pipelineInfo.pVertexInputState = &verterInputStateInfo;
	pipelineInfo.pInputAssemblyState = &assInfo;
	pipelineInfo.pTessellationState = &tessInfo;
	pipelineInfo.pViewportState = &viewportInfo;
	pipelineInfo.pRasterizationState = &rasterStateInfo;
	pipelineInfo.pMultisampleState = &msInfo;
	pipelineInfo.pDepthStencilState = &dsInfo;
	pipelineInfo.pColorBlendState = &blendStateInfo;
	pipelineInfo.pDynamicState = &dynamicStateInfo;
	pipelineInfo.layout = pipelineLayout;
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = 0; // TODO;
	pipelineInfo.basePipelineHandle = nullptr;
	pipelineInfo.basePipelineIndex = 0;

	ASSERT_VK_SUCCESS_ELSE_RET(vkCreateGraphicsPipelines(m_device, pipelineCache, 1, &pipelineInfo, nullptr, &m_pipeline));
}

VulkanGraphicsPipeline::~VulkanGraphicsPipeline()
{
	assert(false);
}

VkPipelineCache VulkanGraphicsPipeline::CreatePipelineCache()
{
	VkPipelineCache pipelineCache{};
	VkPipelineCacheCreateInfo cacheInfo{};
	cacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
	cacheInfo.pNext = nullptr;
	cacheInfo.flags = 0;
	cacheInfo.initialDataSize = 0;
	cacheInfo.pInitialData = nullptr;
	ASSERT_VK_SUCCESS(vkCreatePipelineCache(m_device, &cacheInfo, nullptr, &pipelineCache));

	return pipelineCache;
}

std::vector<VkPipelineShaderStageCreateInfo> VulkanGraphicsPipeline::CreateShaderStages()
{
	uint32_t stageCount = m_params.stagesDescription.size();
	std::vector<VkPipelineShaderStageCreateInfo> stagesInfo(stageCount);
	IVulkanShaderCompiler* compiler = m_services->GetService<IVulkanShaderCompiler>();
	for (int i = 0; i < stagesInfo.size(); ++i)
	{
		ShaderStage stage = m_params.stagesDescription[i].stage;
		std::string shaderPath = m_params.stagesDescription[i].path;
		std::string entryPoint = m_params.stagesDescription[i].entryPoint;
		IVulkanShaderCompiler::CompilationParams shaderParams{};
		shaderParams.device = m_device;
		shaderParams.entryPoint = entryPoint;
		shaderParams.stage = stage;
		shaderParams.sourcePath = shaderPath;
		m_shaderStages[(int)stage] = compiler->CreateShaderFromFile(shaderParams);
		
		stagesInfo[i] = {};
		stagesInfo[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		stagesInfo[i].pNext = nullptr;
		stagesInfo[i].module = m_shaderStages[(int)stage]->GetNativeHandle();
		stagesInfo[i].pName = shaderParams.sourcePath.filename().string().c_str();
		stagesInfo[i].pSpecializationInfo = nullptr;
		stagesInfo[i].stage = VulkanConstantsTranslator::GetVkShaderStage(stage);
	}
	return stagesInfo;
}

VkPipelineVertexInputStateCreateInfo VulkanGraphicsPipeline::CreatePipelineVertexInputStateCreateInfo()
{
	std::vector attrList = CreateAttributeDescriptionList();
	std::vector bindingList = CreateBindingDescriptionList();
	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.pNext = nullptr;
	vertexInputInfo.vertexBindingDescriptionCount = bindingList.size();
	vertexInputInfo.pVertexBindingDescriptions = bindingList.data();
	vertexInputInfo.vertexAttributeDescriptionCount = attrList.size();
	vertexInputInfo.pVertexAttributeDescriptions = attrList.data();

	return vertexInputInfo;
}

VkPipelineInputAssemblyStateCreateInfo VulkanGraphicsPipeline::CreatePipelineInputAssemblyStateCreateInfo()
{
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
	inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyInfo.pNext = nullptr;
	inputAssemblyInfo.topology = VulkanConstantsTranslator::GetVkPrimitiveTopology(m_params.vertexTopology);
	inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
	return inputAssemblyInfo;
}

VkPipelineTessellationStateCreateInfo VulkanGraphicsPipeline::CreatePipelineTessellationStateCreateInfo()
{
	VkPipelineTessellationStateCreateInfo tessStateInfo{};
	tessStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
	tessStateInfo.pNext = nullptr;
	tessStateInfo.patchControlPoints = 0;
	return tessStateInfo;
}

VkPipelineViewportStateCreateInfo VulkanGraphicsPipeline::CreatePipelineViewportStateCreateInfo()
{
	VkViewport viewport{};
	viewport.x = m_params.viewportInfo.x;
	viewport.y = m_params.viewportInfo.y;
	viewport.width = m_params.viewportInfo.width;
	viewport.height = m_params.viewportInfo.height;
	viewport.minDepth = 1;
	viewport.maxDepth = 0;
	VkRect2D scissors;
	scissors.offset = { (int32_t)m_params.viewportInfo.x, (int32_t)m_params.viewportInfo.y };
	scissors.extent = { m_params.viewportInfo.width, m_params.viewportInfo.height };
	VkPipelineViewportStateCreateInfo viewportInfo{};
	viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportInfo.pNext = nullptr;
	viewportInfo.viewportCount = 1;
	viewportInfo.pViewports = &viewport;
	viewportInfo.scissorCount = 1;
	viewportInfo.pScissors = &scissors;

	return viewportInfo;
}

VkPipelineRasterizationStateCreateInfo VulkanGraphicsPipeline::CreatePipelineRasterizationStateCreateInfo()
{
	VkPipelineRasterizationStateCreateInfo rasterStateInfo{};
	rasterStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterStateInfo.pNext = nullptr;
	rasterStateInfo.depthBiasEnable = VK_FALSE;
	rasterStateInfo.frontFace = VulkanConstantsTranslator::GetVkFrontFace(m_params.rasterInfo.frontFace);
	rasterStateInfo.cullMode = VulkanConstantsTranslator::GetVkCullMode(m_params.rasterInfo.cullMode);
	rasterStateInfo.polygonMode = VulkanConstantsTranslator::GetVkPolygonMode(m_params.rasterInfo.polygonMode);
	rasterStateInfo.depthClampEnable = m_params.rasterInfo.enableDepthClamp;
	rasterStateInfo.lineWidth = 1; // Ah i'll add this one later some day;
	return rasterStateInfo;
}

VkPipelineMultisampleStateCreateInfo VulkanGraphicsPipeline::CreatePipelineMultisampleStateCreateInfo()
{
	return VkPipelineMultisampleStateCreateInfo();
}

VkPipelineDepthStencilStateCreateInfo VulkanGraphicsPipeline::CreatePipelineDepthStencilStateCreateInfo()
{
	return VkPipelineDepthStencilStateCreateInfo();
}

VkPipelineColorBlendStateCreateInfo VulkanGraphicsPipeline::CreatePipelineColorBlendStateCreateInfo()
{
	return VkPipelineColorBlendStateCreateInfo();
}

VkPipelineDynamicStateCreateInfo VulkanGraphicsPipeline::CreatePipelineDynamicStateCreateInfo()
{
	return VkPipelineDynamicStateCreateInfo();
}

VkPipelineLayout VulkanGraphicsPipeline::CreatePipelineLayout()
{
	return VkPipelineLayout();
}

VkRenderPass VulkanGraphicsPipeline::CreateRenderPass()
{
	return VkRenderPass();
}

std::vector<VkVertexInputAttributeDescription> VulkanGraphicsPipeline::CreateAttributeDescriptionList()
{
	std::vector<VkVertexInputAttributeDescription> descs(m_params.vertexBufferLayout.size());
	uint32_t offset = 0;
	for (int i = 0; i < m_params.vertexBufferLayout.size(); ++i)
	{
		descs[i].binding = 0;
		descs[i].format = VulkanConstantsTranslator::GetVkFormatForAttribute(m_params.vertexBufferLayout[i]);
		descs[i].location = i;
		descs[i].offset = offset;

		offset += GetAttributeSize(m_params.vertexBufferLayout[i]);
	}
	return descs;
}

std::vector<VkVertexInputBindingDescription> VulkanGraphicsPipeline::CreateBindingDescriptionList()
{
	std::vector<VkVertexInputBindingDescription> descs(1);
	
	uint32_t stride = 0;
	std::for_each(m_params.vertexBufferLayout.begin(), m_params.vertexBufferLayout.end(), [&stride](auto& el) { stride += GetAttributeSize(el); });
	
	descs[0].binding = 0;
	descs[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	descs[0].stride = stride;

	return descs;
}
