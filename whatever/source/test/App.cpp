#include "App.h"
#include "ui/WindowSDL2.h"
#include "vulkan/vulkan.h"
#include "graphics/ISurface.h"
#include "graphics/VkMakros.h"
#include "VulkanAppServiceProvider.h"
#include "graphics/VulkanDescriptorPool.h"
#include <scene/Camera.h>
#include "CameraControler.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_vulkan.h"
#include <graphics/VulkanDevice.h>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/euler_angles.hpp"

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
			.farPlane = 1000.0f,
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

	IFramebuffer::Layout framebufferLayout{};
	framebufferLayout.colorBuffers.emplace_back();
	framebufferLayout.colorBuffers[0] = m_device->GetSwapchainFormat();
	//framebufferLayout.colorBuffers[0].loadOp = AttachmentLoadOp::Clear;
	//framebufferLayout.colorBuffers[0].clearColor = glm::vec4(0.7, 0.5, 0.4, 1);

	IImage::View swapchainImageView(m_device->GetBackbuffer().get());
	m_framebufferInfo.colorBuffers = { swapchainImageView };
	m_framebufferInfo.layout = framebufferLayout;


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

	auto imguiFBLayout = m_framebufferInfo.layout;
	//imguiFBLayout.colorBuffers[0].loadOp = AttachmentLoadOp::Load;
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

void App::Run()
{
	DescriptorSetLayoutParams dsLayoutParams(1);
	dsLayoutParams.DescribeLayoutEntry(0, DescriptorType::UniformBuffer, 1, ShaderStageFlags::Vertex);
	RefPtr<IDescriptorSetLayout> dsLayout = m_device->CreateDescriptorSetLayout(dsLayoutParams);
	IDescriptorSetLayout* dsLayoutRaw = dsLayout.get();

	GraphicsPipelineLayoutCreateInfo layoutCreateInfo{};
	layoutCreateInfo.descriptorSetLayoutCount = 1;
	layoutCreateInfo.descriptorSetLayouts = &dsLayoutRaw;
	RefPtr<IGraphicsPipelineLayout> pipelineLayout = m_device->CreateGraphicsPipelineLayout(layoutCreateInfo);

	IGraphicsPipeline::CreateInfo pipelineInfo{};


	RefPtr<IDescriptorSet> ds = m_descPool->AllocateDescriptorSet(dsLayout.get());

	pipelineInfo.blendStateInfo.attachmentBlendStates.resize(1);
	pipelineInfo.blendStateInfo.attachmentBlendStates[0].blendEnable = true;
	pipelineInfo.blendStateInfo.attachmentBlendStates[0].srcColorBlendFactor = BlendFactor::SrcAlpha;
	pipelineInfo.blendStateInfo.attachmentBlendStates[0].dstColorBlendFactor = BlendFactor::OneMinusSrcAlpha;

	pipelineInfo.depthStencilInfo.depthTestEnable = false;
	pipelineInfo.depthStencilInfo.depthWriteEnable = false;
	pipelineInfo.depthStencilInfo.stencilTestEnabled = false;

	pipelineInfo.rasterInfo.frontFace = FrontFace::Clockwise;
	pipelineInfo.rasterInfo.cullMode = CullMode::None;
	pipelineInfo.rasterInfo.polygonMode = PolygonMode::Fill;

	std::vector<VertexAtributeType> vertexAttributes(1);
	vertexAttributes[0] = VertexAtributeType::float4;
	pipelineInfo.vertexBufferLayout = vertexAttributes;

	pipelineInfo.stagesDescription.resize(2);
	pipelineInfo.stagesDescription[0].stage = ShaderStage::Vertex;
	pipelineInfo.stagesDescription[0].path = "D:\\dev\\pet\\whatever\\whatever\\source\\shaders\\testgraphics.hlsl";
	pipelineInfo.stagesDescription[0].entryPoint = "VS";
	pipelineInfo.stagesDescription[1].stage = ShaderStage::Fragment;
	pipelineInfo.stagesDescription[1].path = "D:\\dev\\pet\\whatever\\whatever\\source\\shaders\\testgraphics.hlsl";
	pipelineInfo.stagesDescription[1].entryPoint = "PS";

	pipelineInfo.framebufferLayout = m_framebufferInfo.layout;
	
	pipelineInfo.vertexTopology = PrimitiveTopology::TriangleList;

	ViewportInfo viewport;
	viewport.x = 0;
	viewport.y = 0;
	viewport.width = m_windowSize.x;
	viewport.height = m_windowSize.y;
	pipelineInfo.viewportInfo = viewport;

	auto graphPipeline = m_device->CreateGraphicsPipeline(pipelineInfo, pipelineLayout);




	auto vp = m_camera->GetViewMatrix() * glm::vec4(-1, 0, 0.5, 0);
	//(0, 0.5, -1, 1)
	// r.x, u.x, -f.x   0 0 1
	// r.y, u.y, -f.y   1 0 0 * (-1 0 0.5) = 0 0.5 -1
	// r.z, u.z, -f.z   0 1 0

	auto cameraUB = m_device->CreateBuffer(IGPUBuffer::CreationParams{
		.bufferSize = sizeof(Camera::CameraCBData),
		.usageFlags = (uint32_t)BufferUsage::UniformBuffer,
		.name = "CameraCB"
	});

	static float vbData[] =
	{
		-1, -0.5, 0, 1,
		-1, 0.5, 0, 1,
		-1, 0, 0.5, 1
	};
	auto vertexBuffer = m_device->CreateBuffer(IGPUBuffer::CreationParams{
		.bufferSize = sizeof(vbData),
		.usageFlags = (uint32_t)BufferUsage::VertexBuffer, 
		.name = "FirstVB"});

	auto vbUpdateCB = m_device->CreateCommandBuffer();
	vbUpdateCB->Begin();
	vbUpdateCB->UpdateBuffer(vertexBuffer.get(), 0, sizeof(vbData), vbData);
	vbUpdateCB->End();
	m_device->Submit(vbUpdateCB.get());

	ds->SetBinding(0, cameraUB.get(), 0, sizeof(Camera::CameraCBData));

	RenderPassParams mainRPParams(m_framebufferInfo.layout);
	mainRPParams.SetColorAttachmentInfo(0, AttachmentLoadOp::Clear, AttachmentStoreOp::Store, ImageLayout::Undefined, ImageLayout::ColorAttachmentOptimal);
	auto mainRP = m_device->CreateRenderPass(mainRPParams);
	while (m_window->IsOpen())
	{
		m_cameraController->Update(0.01);
		m_device->BeginFrame();

		auto commandBuffer = m_device->CreateCommandBuffer();

		auto fbInfo = m_framebufferInfo;
		// Update the backbuffer image view for the current frame
		fbInfo.colorBuffers[0] = IImage::View(m_device->GetBackbuffer().get());
		auto framebuffer = m_device->CreateFramebuffer(std::move(fbInfo));


		size_t vbOffset = 0;
		IGPUBuffer* vertexBuffers[] = { vertexBuffer.get() };
		commandBuffer->Begin();
		commandBuffer->SetPipelineLayout(pipelineLayout.get());
		commandBuffer->SetScissor(Rect2D{ 0, 0, m_windowSize.x, m_windowSize.y });
		Camera::CameraCBData ubData = m_camera->GetConstantData();
		commandBuffer->UpdateBuffer(cameraUB.get(), 0, sizeof(ubData), &ubData);
		commandBuffer->BeginRenderPass(mainRP.get(), framebuffer.get());
		commandBuffer->SetViewport(viewport);
		commandBuffer->BindPipeline(graphPipeline.get());
		commandBuffer->BindVertexBuffers(vertexBuffers, 1, &vbOffset);
		commandBuffer->BindDescriptorSet(0, ds.get());
		commandBuffer->Draw(3, 0, 1);
		commandBuffer->EndRenderPass();

		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Hello");
		ImGui::Text("Hello, Vulkan + SDL2!");
		ImGui::End();

		ImGui::Render();
		ImDrawData* draw_data = ImGui::GetDrawData();
		commandBuffer->BeginRenderPass(m_imguiRenderPass.get(), framebuffer.get());
		ImGui_ImplVulkan_RenderDrawData(draw_data, static_cast<VulkanCommandBuffer*>(commandBuffer.get())->GetNativeHandle());
		commandBuffer->EndRenderPass();
		commandBuffer->End();
		m_device->Submit(commandBuffer.get());
		m_device->Present();
		m_window->Update();
	}
}
