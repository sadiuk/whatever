#include "App.h"
#include "ui/WindowSDL2.h"
#include "vulkan/vulkan.h"
#include "graphics/ISurface.h"
#include "VulkanAppServiceProvider.h"
#include <scene/Camera.h>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_vulkan.h"

using namespace wtv;

void App::Init()
{
	m_services = MakeRef<VulkanAppServiceProvider>();

	IWindow::CreationParams windowParams{};
	windowParams.caption = "EngineTest";
	windowParams.windowPos = m_windowPos;
	windowParams.size = m_windowSize;
	m_window = MakeRef<WindowSDL2>(windowParams);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui_ImplSDL2_InitForVulkan((SDL_Window*)m_window->GetNativeHandle());


	IDevice::CreationParams engineParams{};
	engineParams.api = IDevice::GraphicsAPI::Vulkan;
	engineParams.appName = "EngineTest";
	engineParams.window = m_window;
	m_device = IDevice::Create(engineParams, m_services.get());
}

App::App()
{
	m_windowSize = { 800, 600 };
	m_windowPos = { 300, 200 };
	Init();
}

void App::Run()
{
	IGraphicsPipeline::CreateInfo pipelineInfo{};

	DescriptorSetLayout dsLayout;
	//pipelineInfo.
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

	IFramebuffer::Layout framebufferLayout{};
	framebufferLayout.colorBuffers.emplace_back();
	framebufferLayout.colorBuffers[0].loadOp = AttachmentLoadOp::Clear;
	framebufferLayout.colorBuffers[0].format = m_device->GetSwapchainFormat();
	framebufferLayout.colorBuffers[0].clearColor = glm::vec4(0.7, 0.5, 0.4, 1);
	pipelineInfo.framebufferLayout = framebufferLayout;

	pipelineInfo.stagesDescription.resize(2);
	pipelineInfo.stagesDescription[0].stage = ShaderStage::Vertex;
	pipelineInfo.stagesDescription[0].path = "D:\\dev\\pet\\whatever\\whatever\\source\\shaders\\testgraphics.hlsl";
	pipelineInfo.stagesDescription[0].entryPoint = "VS";
	pipelineInfo.stagesDescription[1].stage = ShaderStage::Fragment;
	pipelineInfo.stagesDescription[1].path = "D:\\dev\\pet\\whatever\\whatever\\source\\shaders\\testgraphics.hlsl";
	pipelineInfo.stagesDescription[1].entryPoint = "PS";
	//pipelineInfo.vertexBufferLayout
	
	pipelineInfo.vertexTopology = PrimitiveTopology::TriangleList;

	ViewportInfo viewport;
	viewport.x = 0;
	viewport.y = 0;
	viewport.width = m_windowSize.x;
	viewport.height = m_windowSize.y;
	pipelineInfo.viewportInfo = viewport;

	auto graphPipeline = m_device->CreateGraphicsPipeline(pipelineInfo);



	Camera camera(Camera::CreationParams{
		.position = glm::vec3(0, 0, 0),
		.normalizedDirection = glm::vec3(1, 0, 0),
		.up = glm::vec3(0, 1, 0),
		.fovYInDegrees = 60,
		.widthToHeightRatio = (float)m_windowSize.x / m_windowSize.y,
		.near = 0.01,
		.far = 1000,
		});

	auto ubData = camera.GetConstantData();
	auto cameraUB = m_device->CreateBuffer(IGPUBuffer::CreationParams{
		.bufferSize = sizeof(ubData),
		.usageFlags = (uint32_t)BufferUsage::UniformBuffer,
		.name = "CameraCB"
	});

	static float vbData[] =
	{
		-0.5, -0.5, 0, 1,
		0.5, -0.5, 0, 1,
		0, 0.5, 0, 1
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

	auto mainRP = m_device->CreateRenderPass(framebufferLayout);
	while (m_window->IsOpen())
	{
		m_device->BeginFrame();

		auto commandBuffer = m_device->CreateCommandBuffer();
		glm::vec4 clearColor(0, 1, 0, 1);

		IFramebuffer::Properties framebufferInfo{};
		IImage::View swapchainImageView(m_device->GetBackbuffer().get());
		framebufferInfo.colorBuffers = { swapchainImageView };
		framebufferInfo.layout = framebufferLayout;
		auto framebuffer = m_device->CreateFramebuffer(std::move(framebufferInfo));

		size_t vbOffset = 0;
		IGPUBuffer* vertexBuffers[] = { vertexBuffer.get() };
		commandBuffer->Begin();
		commandBuffer->SetScissor(Rect2D{ 0, 0, m_windowSize.x, m_windowSize.y });
		commandBuffer->UpdateBuffer(cameraUB.get(), 0, sizeof(ubData), &ubData);
		commandBuffer->BeginRenderPass(mainRP.get(), framebuffer.get());
		commandBuffer->SetClearColorValue(0, &clearColor.x);
		commandBuffer->SetViewport(viewport);
		commandBuffer->BindPipeline(graphPipeline.get());
		commandBuffer->BindVertexBuffers(vertexBuffers, 1, &vbOffset);
		commandBuffer->Draw(3, 0, 1);
		commandBuffer->EndRenderPass();
		commandBuffer->End();

		m_device->Submit(commandBuffer.get());
		m_device->Present();
		m_window->Update();

		//commandBuffer->Reset();
	}
	int a = -0;
}
