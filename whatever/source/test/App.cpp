#include "App.h"
#include "ui/WindowSDL2.h"
#include "vulkan/vulkan.h"
#include "VulkanAppServiceProvider.h"
#include "graphics/Vulkan/VulkanDescriptorPool.h"
#include <scene/Camera.h>
#include "graphics/GPUMesh.h"
#include <set>
#include "CameraControler.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_vulkan.h"
#include <graphics/Vulkan/VulkanDevice.h>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/euler_angles.hpp"
#include "asset/GLTFModelLoader.h"

using namespace wtv;



void App::Init()
{
	m_services = MakeRef<VulkanAppServiceProvider>();

	IWindow::CreationParams windowParams{};
	windowParams.caption = "EngineTest";
	windowParams.windowPos = m_windowPos;
	windowParams.size = m_windowSize;
	m_window = MakeRef<WindowSDL2>(windowParams);

	m_input = MakeRef<Input>(m_window.get());

	m_camera = MakeRef<Camera>(Camera::CreationParams{
			.position = glm::vec3(0, 0, 0),
			.normalizedDirection = glm::vec3(-1, 0, 0),
			.up = glm::vec3(0, 0, 1),
			.fovYInDegrees = 60,
			.widthToHeightRatio = (float)m_windowSize.x / m_windowSize.y,
			.nearPlane = 0.01f,
			.farPlane = 10000.0f,
		});

	m_cameraController = MakeRef<CameraController>(m_services.get(), m_input, m_camera);

	IDevice::CreationParams engineParams{};
	engineParams.api = IDevice::GraphicsAPI::Vulkan;
	engineParams.appName = "EngineTest";
	engineParams.window = m_window;
	m_device = IDevice::Create(engineParams, m_services.get());

	DescriptorPoolParams params;
	params.maxSetCount = 10000;
	params.SetDescriptorCount(DescriptorType::Sampler, 1000);
	params.SetDescriptorCount(DescriptorType::CombinedImageSampler, 1000);
	params.SetDescriptorCount(DescriptorType::SampledImage, 1000);
	params.SetDescriptorCount(DescriptorType::StorageImage, 1000);
	params.SetDescriptorCount(DescriptorType::UniformTexelBuffer, 1000);
	params.SetDescriptorCount(DescriptorType::StorageTexelBuffer, 1000);
	params.SetDescriptorCount(DescriptorType::UniformBuffer, 1000);
	params.SetDescriptorCount(DescriptorType::UniformBufferDynamic, 1000);
	params.SetDescriptorCount(DescriptorType::StorageBufferDynamic, 1000);
	params.SetDescriptorCount(DescriptorType::InputAttachment, 1000);
	m_descPool = m_device->CreateDescriptorPool(params);

	m_cpuToGpuConverter = MakeRef<CPUtoGPUConverter>(m_services.get(), m_device.get());

	IImage::CreationParams depthBufferParams{ m_windowSize.x, m_windowSize.y, 1, 1, 1, ImageFormat::D32_SFLOAT, ImageDimension::Dimension2D, 1, ImageUsage::DepthStencilAttachment };
	m_depthBuffer = m_device->CreateImage(depthBufferParams, MemoryPropertyFlags::DeviceLocal, "Scene Depth Buffer");

	IFramebuffer::Layout framebufferLayout{};
	framebufferLayout.colorBuffers.emplace_back();
	framebufferLayout.colorBuffers[0] = m_device->GetSwapchainFormat();
	framebufferLayout.depthBuffer = m_depthBuffer->GetProperties().format;

	IImage::View swapchainImageView(m_device->GetBackbuffer(), ImageAspectFlags::ColorBit);
	IImage::View depthBufferImageView(m_depthBuffer.get(), ImageAspectFlags::DepthBit);
	m_framebufferInfo.colorBuffers = { swapchainImageView };
	m_framebufferInfo.depthBuffer = depthBufferImageView;
	m_framebufferInfo.layout = framebufferLayout;

	IFramebuffer::Layout imaguiFramebufferLayout{};
	imaguiFramebufferLayout.colorBuffers.emplace_back();
	imaguiFramebufferLayout.colorBuffers[0] = m_device->GetSwapchainFormat();
	m_imguiFramebufferInfo.colorBuffers = { swapchainImageView };
	m_imguiFramebufferInfo.layout = imaguiFramebufferLayout;


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui_ImplSDL2_InitForVulkan((SDL_Window*)m_window->GetNativeHandle());

	VulkanDevice* vulkanDevice = static_cast<VulkanDevice*>(m_device.get());
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = vulkanDevice->GetInstance();
	init_info.PhysicalDevice = vulkanDevice->GetPhysicalDevice();
	init_info.Device = vulkanDevice->GetDevice();
	init_info.QueueFamily = vulkanDevice->GetQueueFamilyIndices().graphicsFamilyIndex.value();
	init_info.Queue = vulkanDevice->GetGraphicsQueue()->GetNativeHandle();
	init_info.PipelineCache = VK_NULL_HANDLE;
	init_info.DescriptorPool = static_cast<VulkanDescriptorPool*>(m_descPool.get())->GetNativeHandle();
	init_info.Allocator = nullptr;
	init_info.MinImageCount = 3;
	init_info.ImageCount = vulkanDevice->GetSwapchain()->GetImageCount();
	init_info.CheckVkResultFn = [](VkResult err) {
		if (err != VK_SUCCESS) {
			fprintf(stderr, "Vulkan error: %d\n", err);
		}
		};

	IFramebuffer::Layout imguiFBLayout{};
	imguiFBLayout.colorBuffers.emplace_back();
	imguiFBLayout.colorBuffers[0] = m_device->GetSwapchainFormat();

	
	RenderPassParams imguiRPParams(imguiFBLayout);
	imguiRPParams.SetColorAttachmentInfo(0, AttachmentLoadOp::Load, AttachmentStoreOp::Store, ImageLayout::ColorAttachmentOptimal, ImageLayout::PresentSrcKhr);
	m_imguiRenderPass = m_device->CreateRenderPass(imguiRPParams);
	init_info.PipelineInfoMain.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
	init_info.PipelineInfoMain.RenderPass = static_cast<VulkanRenderPass*>(m_imguiRenderPass.get())->GetNativeHandle();
	init_info.PipelineInfoMain.Subpass = 0;
	init_info.PipelineInfoMain.PipelineRenderingCreateInfo.colorAttachmentCount = 1;
	VkFormat colorFmt = VulkanConstantTranslator::GetVkFormat(m_framebufferInfo.layout.colorBuffers[0]);
	init_info.PipelineInfoMain.PipelineRenderingCreateInfo.pColorAttachmentFormats = &colorFmt;
	init_info.PipelineInfoMain.PipelineRenderingCreateInfo.depthAttachmentFormat = VK_FORMAT_UNDEFINED;
	init_info.PipelineInfoMain.PipelineRenderingCreateInfo.stencilAttachmentFormat = VK_FORMAT_UNDEFINED;
	init_info.PipelineInfoMain.PipelineRenderingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;

	ImGui_ImplVulkan_Init(&init_info);


}

App::App()
{
	m_windowSize = { 800, 600 };
	m_windowPos = { 300, 200 };
	Init();
}

App::~App()
{
	vkDeviceWaitIdle(static_cast<VulkanDevice*>(m_device.get())->GetDevice());
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

struct PushConstants
{
	uint32_t diffuseTexIndex;
	uint32_t normalTexIndex;
	uint32_t metalicRoughnessTexIndex;
	uint32_t emissiveTexIndex;
};

void App::Run()
{
	DescriptorSetLayoutParams dsLayoutParams(1);
	dsLayoutParams.DescribeLayoutEntry(0, DescriptorType::UniformBuffer, 1, ShaderStageFlags::Vertex);
	RefPtr<IDescriptorSetLayout> dsLayout = m_device->CreateDescriptorSetLayout(dsLayoutParams);
	IDescriptorSetLayout* dsLayoutRaw = dsLayout.get();



	auto graphicsQueue = m_device->GetGraphicsQueue();



	GLTFModelLoader loader;
	auto cpuMeshes = loader.LoadModel("D:/dev/pet/whatever/whatever/media/Sponza/glTF/Sponza.gltf");
	
	auto gpuMeshes = m_cpuToGpuConverter->ConvertToGPU(cpuMeshes, m_descPool.get());
	auto matDescSet = gpuMeshes.begin()->second[0].GetMaterial()->m_descriptorSet;

	GraphicsPipelineLayoutCreateInfo layoutCreateInfo{};
	layoutCreateInfo.descriptorSetLayoutCount = 2;
	IDescriptorSetLayout* layouts[2] = { dsLayoutRaw, matDescSet->GetLayout() };
	layoutCreateInfo.descriptorSetLayouts = layouts;
	layoutCreateInfo.pushConstantRangeCount = 1;
	PushConstantRange range;
	range.offset = 0;
	range.size = sizeof(PushConstants);
	range.stages = ShaderStageFlags::All;
	layoutCreateInfo.pcRanges = &range;

	RefPtr<IGraphicsPipelineLayout> pipelineLayout = m_device->CreateGraphicsPipelineLayout(layoutCreateInfo);

	std::vector<std::vector<VertexAttributeType>> vertexAttributes(gpuMeshes.size());
	int attrSetIndex = 0;
	for(auto& meshSet : gpuMeshes)
	{ 
		for (auto& attr : meshSet.first)
		{
			if(attr.semantic != VertexAttributeSemantic::Undefined)
				vertexAttributes[attrSetIndex].push_back(attr.type);
		}
		attrSetIndex++;
	}

	
	RefPtr<IDescriptorSet> ds = m_descPool->AllocateDescriptorSet(dsLayout);

	ViewportInfo viewport;
	viewport.x = 0;
	viewport.y = 0;
	viewport.width = m_windowSize.x;
	viewport.height = m_windowSize.y;
	std::vector<RefPtr<IGraphicsPipeline>> graphPipelines(gpuMeshes.size());
	for (int i = 0; i < vertexAttributes.size(); i++)
	{
		IGraphicsPipeline::CreateInfo pipelineInfo{};
		pipelineInfo.blendStateInfo.attachmentBlendStates.resize(1);
		pipelineInfo.blendStateInfo.attachmentBlendStates[0].blendEnable = true;
		pipelineInfo.blendStateInfo.attachmentBlendStates[0].srcColorBlendFactor = BlendFactor::SrcAlpha;
		pipelineInfo.blendStateInfo.attachmentBlendStates[0].dstColorBlendFactor = BlendFactor::OneMinusSrcAlpha;

		pipelineInfo.depthStencilInfo.depthTestEnable = true;
		pipelineInfo.depthStencilInfo.depthWriteEnable = true;
		pipelineInfo.depthStencilInfo.stencilTestEnabled = false;
		pipelineInfo.depthStencilInfo.depthTestPassResult = CompareOperation::Greater;

		pipelineInfo.rasterInfo.frontFace = FrontFace::CounterClockwise;
		pipelineInfo.rasterInfo.cullMode = CullMode::None;
		pipelineInfo.rasterInfo.polygonMode = PolygonMode::Fill;


		pipelineInfo.vertexBufferLayout = vertexAttributes[i];


		pipelineInfo.stagesDescription.resize(2);
		pipelineInfo.stagesDescription[0].stage = ShaderStage::Vertex;
		pipelineInfo.stagesDescription[0].path = "D:\\dev\\pet\\whatever\\whatever\\source\\shaders\\testgraphics.hlsl";
		pipelineInfo.stagesDescription[0].entryPoint = "VS";
		pipelineInfo.stagesDescription[1].stage = ShaderStage::Fragment;
		pipelineInfo.stagesDescription[1].path = "D:\\dev\\pet\\whatever\\whatever\\source\\shaders\\testgraphics.hlsl";
		pipelineInfo.stagesDescription[1].entryPoint = "PS";

		pipelineInfo.framebufferLayout = m_framebufferInfo.layout;

		pipelineInfo.vertexTopology = PrimitiveTopology::TriangleList;

		pipelineInfo.viewportInfo = viewport;


		graphPipelines[i] = m_device->CreateGraphicsPipeline(pipelineInfo, pipelineLayout);
	}


	auto vp = m_camera->GetViewMatrix() * glm::vec4(-1, 0, 0.5, 0);
	//(0, 0.5, -1, 1)
	// r.x, u.x, -f.x   0 0 1
	// r.y, u.y, -f.y   1 0 0 * (-1 0 0.5) = 0 0.5 -1
	// r.z, u.z, -f.z   0 1 0

	auto cameraUB = m_device->CreateBuffer(IGPUBuffer::CreationParams{
		.bufferSize = sizeof(Camera::CameraCBData),
		.usageFlags = BufferUsage::UniformBuffer
	}, "CameraCB");

	
	ds->SetBinding(0, cameraUB.get(), 0, sizeof(Camera::CameraCBData));

	RenderPassParams mainRPParams(m_framebufferInfo.layout);
	glm::vec4 clearColor(0, 0, 0, 0);
	float clearDepth = 0;
	mainRPParams.SetColorAttachmentInfo(0, AttachmentLoadOp::Clear, AttachmentStoreOp::Store, ImageLayout::Undefined, ImageLayout::ColorAttachmentOptimal, &clearColor);
	mainRPParams.SetDepthAttachmentInfo(AttachmentLoadOp::Clear, AttachmentStoreOp::Store, ImageLayout::Undefined, ImageLayout::DepthStencilAttachmentOptimal, &clearDepth);
	auto mainRP = m_device->CreateRenderPass(mainRPParams);
	while (m_window->IsOpen())
	{
		m_cameraController->Update(0.01);
		m_device->BeginFrame();

		auto commandBuffer = m_device->CreateCommandBuffer();

		auto fbInfo = m_framebufferInfo;
		// Update the backbuffer image view for the current frame
		fbInfo.colorBuffers[0] = IImage::View(m_device->GetBackbuffer(), ImageAspectFlags::ColorBit);
		fbInfo.depthBuffer = IImage::View(m_depthBuffer.get(), ImageAspectFlags::DepthBit);
		auto framebuffer = m_device->CreateFramebuffer(std::move(fbInfo));


		size_t vbOffset = 0;
		commandBuffer->Begin();
		commandBuffer->SetPipelineLayout(pipelineLayout.get());
		commandBuffer->SetScissor(Rect2D{ 0, 0, m_windowSize.x, m_windowSize.y });
		Camera::CameraCBData ubData = m_camera->GetConstantData();
		commandBuffer->UpdateBuffer(cameraUB.get(), 0, sizeof(ubData), &ubData);
		commandBuffer->BeginRenderPass(mainRP.get(), framebuffer.get());
		commandBuffer->SetViewport(viewport);
		commandBuffer->BindDescriptorSet(0, ds.get());
		commandBuffer->BindDescriptorSet(1, matDescSet.get());
		int pipelineIndex = 0;
		for (auto& meshSet : gpuMeshes)
		{
			commandBuffer->BindPipeline(graphPipelines[pipelineIndex].get());

			for (auto& mesh : meshSet.second)
			{
				PushConstants pc{};
				pc.diffuseTexIndex = mesh.GetMaterial()->m_diffuseTexture.m_indexInDescriptorSet;
				pc.normalTexIndex = mesh.GetMaterial()->m_normalTexture.m_indexInDescriptorSet;
				pc.metalicRoughnessTexIndex = mesh.GetMaterial()->m_metallicRoughnessTexture.m_indexInDescriptorSet;
				pc.emissiveTexIndex = mesh.GetMaterial()->m_emissiveTexture.m_indexInDescriptorSet;
				commandBuffer->PushConstants(&pc, sizeof(pc), ShaderStageFlags::All);
				IGPUBuffer* vertexBuffer = mesh.GetVertexBuffer();
				commandBuffer->BindVertexBuffers(&vertexBuffer, 1, &vbOffset);
				IGPUBuffer* indexBuffer = mesh.GetIndexBuffer();
				commandBuffer->BindIndexBuffer(indexBuffer, 0, mesh.GetMeshInfo().indexType);
				uint32_t indexCount = mesh.GetIndexBuffer()->GetSize() / GetIndexSize(mesh.GetMeshInfo().indexType);
				commandBuffer->DrawIndexed(0, 0, indexCount, 1, 0);
			}
			pipelineIndex++;
		}

		commandBuffer->EndRenderPass();

		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Hello");
		ImGui::Text("Hello, Vulkan + SDL2!");
		auto pos = m_camera->GetPosition();
		ImGui::Text("Camera Position: x=%.2f, y=%.2f, z=%.2f", pos.x, pos.y, pos.z);
		ImGui::End();
		ImGui::Render();

		auto imguiFbInfo = m_imguiFramebufferInfo;
		// Update the backbuffer image view for the current frame
		imguiFbInfo.colorBuffers[0] = IImage::View(m_device->GetBackbuffer(), ImageAspectFlags::ColorBit);
		auto imguiFB = m_device->CreateFramebuffer(std::move(imguiFbInfo));
		ImDrawData* draw_data = ImGui::GetDrawData();
		commandBuffer->BeginRenderPass(m_imguiRenderPass.get(), imguiFB.get());
		ImGui_ImplVulkan_RenderDrawData(draw_data, static_cast<VulkanCommandBuffer*>(commandBuffer.get())->GetNativeHandle());
		commandBuffer->EndRenderPass();
		commandBuffer->End();
		{
			QueueSubmitInfo submitInfo(commandBuffer.get(), m_device.get());
			graphicsQueue->Submit(submitInfo);
		}
		m_device->Present();
		m_window->Update();
	}
}
