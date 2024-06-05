#include "VulkanGraphicsPipeline.h"
#include "VulkanEngine.h"
#include "VkMakros.h"
#include "VulkanConstantTranslator.h"
#include "IVulkanShaderCompiler.h"
namespace wtv
{
	VulkanGraphicsPipeline::VulkanGraphicsPipeline(VulkanEngine* engine, IServiceProvider* services, const CreateInfo& params) :
		IGraphicsPipeline(params),
		m_engine(engine),
		m_services(services)
	{
		m_pipelineCache = CreatePipelineCache();
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages = CreateShaderStages();

		std::vector attrList = CreateAttributeDescriptionList();
		std::vector bindingList = CreateBindingDescriptionList();
		VkPipelineVertexInputStateCreateInfo verterInputStateInfo = CreatePipelineVertexInputStateCreateInfo(attrList, bindingList);
		VkPipelineInputAssemblyStateCreateInfo assInfo = CreatePipelineInputAssemblyStateCreateInfo(); // Totally intentional
		VkPipelineTessellationStateCreateInfo tessInfo = CreatePipelineTessellationStateCreateInfo();

		VkViewport viewport{};
		viewport.x = (float)m_params.viewportInfo.x;
		viewport.y = (float)m_params.viewportInfo.y;
		viewport.width = (float)m_params.viewportInfo.width;
		viewport.height = (float)m_params.viewportInfo.height;
		viewport.minDepth = 1;
		viewport.maxDepth = 0;
		VkRect2D scissors;
		scissors.offset = { (int32_t)m_params.viewportInfo.x, (int32_t)m_params.viewportInfo.y };
		scissors.extent = { m_params.viewportInfo.width, m_params.viewportInfo.height };
		VkPipelineViewportStateCreateInfo viewportInfo = CreatePipelineViewportStateCreateInfo(viewport, scissors);

		VkPipelineRasterizationStateCreateInfo rasterStateInfo = CreatePipelineRasterizationStateCreateInfo();
		VkPipelineMultisampleStateCreateInfo msInfo = CreatePipelineMultisampleStateCreateInfo();
		VkPipelineDepthStencilStateCreateInfo dsInfo = CreatePipelineDepthStencilStateCreateInfo();
		std::vector<VkPipelineColorBlendAttachmentState> attachmentStates;
		VkPipelineColorBlendStateCreateInfo blendStateInfo = CreatePipelineColorBlendStateCreateInfo(attachmentStates);
		VkPipelineDynamicStateCreateInfo dynamicStateInfo = CreatePipelineDynamicStateCreateInfo();
		VkPipelineLayout pipelineLayout = CreatePipelineLayout();
		VkRenderPass renderPass = CreateRenderPass();

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.pNext = nullptr;
		pipelineInfo.flags = 0;
		pipelineInfo.stageCount = (uint32_t)shaderStages.size();
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

		ASSERT_VK_SUCCESS_ELSE_RET(vkCreateGraphicsPipelines(m_engine->GetDevice(), m_pipelineCache, 1, &pipelineInfo, nullptr, &m_pipeline));
	}

	RefPtr<IFramebuffer> VulkanGraphicsPipeline::CreateFramebuffer(const IFramebuffer::CreateInfo& params)
	{
		return static_cast<VulkanEngine*>(m_engine)->CreateFramebuffer(params, m_renderPass);
	}

	VulkanGraphicsPipeline::~VulkanGraphicsPipeline()
	{
		vkDestroyPipeline(m_engine->GetDevice(), m_pipeline, nullptr);
		vkDestroyRenderPass(m_engine->GetDevice(), m_renderPass, nullptr);
		vkDestroyPipelineCache(m_engine->GetDevice(), m_pipelineCache, nullptr);
		vkDestroyPipelineLayout(m_engine->GetDevice(), m_pipelineLayout, nullptr);

	}

	IServiceProvider* VulkanGraphicsPipeline::GetServiceProvider()
	{
		return m_engine->GetServiceProvider();
	}

	VkPipelineCache VulkanGraphicsPipeline::CreatePipelineCache()
	{
		VkPipelineCache cache;
		VkPipelineCacheCreateInfo cacheInfo{};
		cacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		cacheInfo.pNext = nullptr;
		cacheInfo.flags = 0;
		cacheInfo.initialDataSize = 0;
		cacheInfo.pInitialData = nullptr;
		ASSERT_VK_SUCCESS(vkCreatePipelineCache(m_engine->GetDevice(), &cacheInfo, nullptr, &cache));
		return cache;
	}

	std::vector<VkPipelineShaderStageCreateInfo> VulkanGraphicsPipeline::CreateShaderStages()
	{
		uint32_t stageCount = (uint32_t)m_params.stagesDescription.size();
		std::vector<VkPipelineShaderStageCreateInfo> stagesInfo(stageCount);
		IVulkanShaderCompiler* compiler = m_services->GetService<IVulkanShaderCompiler>();
		for (int i = 0; i < stagesInfo.size(); ++i)
		{
			ShaderStage stage = m_params.stagesDescription[i].stage;
			std::string shaderPath = m_params.stagesDescription[i].path;
			std::string entryPoint = m_params.stagesDescription[i].entryPoint;
			IVulkanShaderCompiler::CompilationParams shaderParams{};
			shaderParams.engine = m_engine;
			shaderParams.entryPoint = entryPoint;
			shaderParams.stage = stage;
			shaderParams.sourcePath = shaderPath;
			m_shaderStages[(int)stage] = compiler->CreateShaderFromFile(shaderParams);

			stagesInfo[i] = {};
			stagesInfo[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			stagesInfo[i].pNext = nullptr;
			stagesInfo[i].module = m_shaderStages[(int)stage]->GetNativeHandle();
			stagesInfo[i].pName = m_params.stagesDescription[i].entryPoint.c_str();
			stagesInfo[i].pSpecializationInfo = nullptr;
			stagesInfo[i].stage = VulkanConstantTranslator::GetVkShaderStage(stage);
		}
		return stagesInfo;
	}

	VkPipelineVertexInputStateCreateInfo VulkanGraphicsPipeline::CreatePipelineVertexInputStateCreateInfo(
		std::vector<VkVertexInputAttributeDescription>& attrList,
		std::vector<VkVertexInputBindingDescription>& bindingList
	)
	{
		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.pNext = nullptr;
		vertexInputInfo.vertexBindingDescriptionCount = (uint32_t)bindingList.size();
		vertexInputInfo.pVertexBindingDescriptions = bindingList.data();
		vertexInputInfo.vertexAttributeDescriptionCount = (uint32_t)attrList.size();
		vertexInputInfo.pVertexAttributeDescriptions = attrList.data();

		return vertexInputInfo;
	}

	VkPipelineInputAssemblyStateCreateInfo VulkanGraphicsPipeline::CreatePipelineInputAssemblyStateCreateInfo()
	{
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
		inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyInfo.pNext = nullptr;
		inputAssemblyInfo.topology = VulkanConstantTranslator::GetVkPrimitiveTopology(m_params.vertexTopology);
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

	VkPipelineViewportStateCreateInfo VulkanGraphicsPipeline::CreatePipelineViewportStateCreateInfo(
		VkViewport& viewport,
		VkRect2D& scissors
	)
	{
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
		rasterStateInfo.frontFace = VulkanConstantTranslator::GetVkFrontFace(m_params.rasterInfo.frontFace);
		rasterStateInfo.cullMode = VulkanConstantTranslator::GetVkCullMode(m_params.rasterInfo.cullMode);
		rasterStateInfo.polygonMode = VulkanConstantTranslator::GetVkPolygonMode(m_params.rasterInfo.polygonMode);
		rasterStateInfo.depthClampEnable = m_params.rasterInfo.enableDepthClamp;
		rasterStateInfo.lineWidth = 1; // Ah i'll add this one later some day;
		return rasterStateInfo;
	}

	VkPipelineMultisampleStateCreateInfo VulkanGraphicsPipeline::CreatePipelineMultisampleStateCreateInfo()
	{
		VkPipelineMultisampleStateCreateInfo msaaInfo{};
		msaaInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		msaaInfo.pNext = nullptr;
		msaaInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		msaaInfo.sampleShadingEnable = VK_FALSE;
		msaaInfo.minSampleShading = 0.2f;
		msaaInfo.pSampleMask = 0;
		msaaInfo.alphaToCoverageEnable = VK_FALSE;
		msaaInfo.alphaToOneEnable = VK_FALSE;
		return msaaInfo;

	}

	VkPipelineDepthStencilStateCreateInfo VulkanGraphicsPipeline::CreatePipelineDepthStencilStateCreateInfo()
	{
		VkPipelineDepthStencilStateCreateInfo dsInfo{};
		dsInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		dsInfo.pNext = nullptr;
		dsInfo.flags = 0;
		dsInfo.depthTestEnable = m_params.depthStencilInfo.depthTestEnable ? VK_TRUE : VK_FALSE;
		dsInfo.depthWriteEnable = m_params.depthStencilInfo.depthWriteEnable ? VK_TRUE : VK_FALSE;
		dsInfo.depthCompareOp = VulkanConstantTranslator::GetCompareOperation(m_params.depthStencilInfo.depthTestPassResult);
		dsInfo.depthBoundsTestEnable = VK_FALSE;
		dsInfo.minDepthBounds = 0; // Because depth bounds test is disabled
		dsInfo.maxDepthBounds = 0; // Because depth bounds test is disabled
		dsInfo.stencilTestEnable = VK_FALSE;// m_params.depthStencilInfo.stencilTestEnabled ? VK_TRUE : VK_FALSE;
		dsInfo.front.failOp = VulkanConstantTranslator::GetStencilTestOperation(m_params.depthStencilInfo.frontStencilOp.failOperation);
		dsInfo.front.passOp = VulkanConstantTranslator::GetStencilTestOperation(m_params.depthStencilInfo.frontStencilOp.passOperation);
		dsInfo.front.depthFailOp = VulkanConstantTranslator::GetStencilTestOperation(m_params.depthStencilInfo.frontStencilOp.depthFailOperation);
		dsInfo.front.compareOp = VulkanConstantTranslator::GetCompareOperation(m_params.depthStencilInfo.frontStencilOp.compareOperation);
		dsInfo.front.writeMask = m_params.depthStencilInfo.frontStencilOp.writeMask;
		dsInfo.front.compareMask = m_params.depthStencilInfo.frontStencilOp.compareMask;
		dsInfo.front.reference = m_params.depthStencilInfo.frontStencilOp.reference;

		dsInfo.back.failOp = VulkanConstantTranslator::GetStencilTestOperation(m_params.depthStencilInfo.backStencilOp.failOperation);
		dsInfo.back.passOp = VulkanConstantTranslator::GetStencilTestOperation(m_params.depthStencilInfo.backStencilOp.passOperation);
		dsInfo.back.depthFailOp = VulkanConstantTranslator::GetStencilTestOperation(m_params.depthStencilInfo.backStencilOp.depthFailOperation);
		dsInfo.back.compareOp = VulkanConstantTranslator::GetCompareOperation(m_params.depthStencilInfo.backStencilOp.compareOperation);
		dsInfo.back.writeMask = m_params.depthStencilInfo.backStencilOp.writeMask;
		dsInfo.back.compareMask = m_params.depthStencilInfo.backStencilOp.compareMask;
		dsInfo.back.reference = m_params.depthStencilInfo.backStencilOp.reference;

		return dsInfo;
	}

	VkPipelineColorBlendStateCreateInfo VulkanGraphicsPipeline::CreatePipelineColorBlendStateCreateInfo(
		std::vector<VkPipelineColorBlendAttachmentState>& attachmentStates
	)
	{
		attachmentStates.resize(m_params.blendStateInfo.attachmentBlendStates.size());
		for (int i = 0; i < attachmentStates.size(); ++i)
		{
			auto& s = m_params.blendStateInfo.attachmentBlendStates[i];
			attachmentStates[i] = {};
			attachmentStates[i].blendEnable = TOVKBOOL(s.blendEnable);
			attachmentStates[i].srcColorBlendFactor = VulkanConstantTranslator::GetVkBlendFactor(s.srcColorBlendFactor);
			attachmentStates[i].dstColorBlendFactor = VulkanConstantTranslator::GetVkBlendFactor(s.dstColorBlendFactor);
			attachmentStates[i].colorBlendOp = VulkanConstantTranslator::GetVkBlendOp(s.colorBlendOperation);
			attachmentStates[i].srcAlphaBlendFactor = VulkanConstantTranslator::GetVkBlendFactor(s.srcAlphaBlendFactor);
			attachmentStates[i].dstAlphaBlendFactor = VulkanConstantTranslator::GetVkBlendFactor(s.dstAlphaBlendFactor);
			attachmentStates[i].alphaBlendOp = VulkanConstantTranslator::GetVkBlendOp(s.alphaBlendOperation);
			attachmentStates[i].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		}

		float blendConstants[4] = { 0, 0, 0, 0 };
		VkPipelineColorBlendStateCreateInfo blendInfo{};
		blendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		blendInfo.pNext = nullptr;
		blendInfo.attachmentCount = (uint32_t)attachmentStates.size();
		blendInfo.pAttachments = attachmentStates.data();
		blendInfo.logicOpEnable = VK_FALSE;
		blendInfo.blendConstants[0] = blendConstants[0];
		blendInfo.blendConstants[1] = blendConstants[1];
		blendInfo.blendConstants[2] = blendConstants[2];
		blendInfo.blendConstants[3] = blendConstants[3];
		return blendInfo;
	}

	VkPipelineDynamicStateCreateInfo VulkanGraphicsPipeline::CreatePipelineDynamicStateCreateInfo()
	{
		VkPipelineDynamicStateCreateInfo dynStateInfo{};
		dynStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynStateInfo.pNext = nullptr;
		dynStateInfo.dynamicStateCount = 0;

		return dynStateInfo;
	}

	VkPipelineLayout VulkanGraphicsPipeline::CreatePipelineLayout()
	{
		m_pushConstantRanges = CreatePushConstantRanges();
		m_descSetLayouts = CreateDescriptorSetLayouts();
		VkPipelineLayout layout{};
		VkPipelineLayoutCreateInfo layoutCreateInfo{};
		layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		layoutCreateInfo.pNext = nullptr;
		layoutCreateInfo.pushConstantRangeCount = (uint32_t)m_pushConstantRanges.size();
		layoutCreateInfo.pPushConstantRanges = m_pushConstantRanges.data();
		layoutCreateInfo.setLayoutCount = (uint32_t)m_descSetLayouts.size();
		layoutCreateInfo.pSetLayouts = m_descSetLayouts.data();

		ASSERT_VK_SUCCESS(vkCreatePipelineLayout(m_engine->GetDevice(), &layoutCreateInfo, nullptr, &layout));
		return layout;
	}

	VkRenderPass VulkanGraphicsPipeline::CreateRenderPass()
	{
		std::vector<IFramebuffer::RenderTargetInfo> allRenderTargets(m_params.framebufferLayout.colorBuffers.begin(), m_params.framebufferLayout.colorBuffers.end());
		allRenderTargets.insert(allRenderTargets.end(), m_params.framebufferLayout.depthBuffers.begin(), m_params.framebufferLayout.depthBuffers.end());
		std::vector<VkAttachmentDescription> attachmentDescs(allRenderTargets.size());
		for (int i = 0; i < attachmentDescs.size(); ++i)
		{
			// TODO: image layouts, stencil stuff
			attachmentDescs[i] = {};
			attachmentDescs[i].format = VulkanConstantTranslator::GetVkFormat(allRenderTargets[i].format);
			attachmentDescs[i].loadOp = allRenderTargets[i].clearBeforeWrite ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
			attachmentDescs[i].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			attachmentDescs[i].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			attachmentDescs[i].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			attachmentDescs[i].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			attachmentDescs[i].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			attachmentDescs[i].samples = VK_SAMPLE_COUNT_1_BIT;
		}
		VkAttachmentReference attachmentRef{};
		attachmentRef.attachment = 0;
		attachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		VkSubpassDescription subpassDesc{};
		subpassDesc.colorAttachmentCount = 1;
		subpassDesc.pColorAttachments = &attachmentRef;
		subpassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;


		VkRenderPassCreateInfo rpInfo{};
		rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		rpInfo.pNext = nullptr;
		rpInfo.attachmentCount = (uint32_t)attachmentDescs.size();
		rpInfo.pAttachments = attachmentDescs.data();
		rpInfo.dependencyCount = 0;
		rpInfo.pDependencies = nullptr;
		rpInfo.subpassCount = 1;
		rpInfo.pSubpasses = &subpassDesc;

		ASSERT_VK_SUCCESS_ELSE_RET0(vkCreateRenderPass(m_engine->GetDevice(), &rpInfo, nullptr, &m_renderPass));
		return m_renderPass;
	}

	std::vector<VkVertexInputAttributeDescription> VulkanGraphicsPipeline::CreateAttributeDescriptionList()
	{
		std::vector<VkVertexInputAttributeDescription> descs(m_params.vertexBufferLayout.size());
		uint32_t offset = 0;
		for (int i = 0; i < m_params.vertexBufferLayout.size(); ++i)
		{
			descs[i].binding = 0;
			descs[i].format = VulkanConstantTranslator::GetVkFormatForAttribute(m_params.vertexBufferLayout[i]);
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

	std::vector<VkPushConstantRange> VulkanGraphicsPipeline::CreatePushConstantRanges()
	{
		return std::vector<VkPushConstantRange>();
	}

	std::vector<VkDescriptorSetLayout> VulkanGraphicsPipeline::CreateDescriptorSetLayouts()
	{
		VkDescriptorSetLayoutCreateInfo dslInfo{};
		dslInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		dslInfo.pNext = nullptr;
		dslInfo.bindingCount = 0;
		dslInfo.pBindings = nullptr;


		return std::vector<VkDescriptorSetLayout>();
	}

}