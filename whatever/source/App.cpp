#include "App.h"
#include "ui/WindowSDL2.h"
#include "vulkan/vulkan.h"
#include "graphics/ISurface.h"
#include "VulkanAppServiceProvider.h"
using namespace wtv;

void App::Init()
{
	m_services = MakeRef<VulkanAppServiceProvider>();

	IWindow::CreationParams windowParams{};
	windowParams.caption = "EngineTest";
	windowParams.windowPos = m_windowPos;
	windowParams.size = m_windowSize;
	m_window = MakeRef<WindowSDL2>(windowParams);

	IEngine::CreationParams engineParams{};
	engineParams.api = IEngine::GraphicsAPI::Vulkan;
	engineParams.appName = "EngineTest";
	engineParams.window = m_window;
	m_graphicsEngine = IEngine::Create(engineParams, m_services.get());
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
	pipelineInfo.blendStateInfo.attachmentBlendStates.resize(1);
	pipelineInfo.blendStateInfo.attachmentBlendStates[0].blendEnable = true;

	pipelineInfo.depthStencilInfo.depthTestEnable = false;
	pipelineInfo.depthStencilInfo.depthWriteEnable = false;
	pipelineInfo.depthStencilInfo.stencilTestEnabled = false;

	pipelineInfo.rasterInfo.frontFace = FrontFace::Clockwise;
	pipelineInfo.rasterInfo.cullMode = CullMode::None;
	pipelineInfo.rasterInfo.polygonMode = PolygonMode::Fill;

	IFramebuffer::Layout framebufferLayout{};
	framebufferLayout.colorBuffers.emplace_back();
	framebufferLayout.colorBuffers[0].clearBeforeWrite = true;
	framebufferLayout.colorBuffers[0].format = m_graphicsEngine->GetSwapchainFormat();
	pipelineInfo.framebufferLayout = framebufferLayout;

	pipelineInfo.stagesDescription.resize(2);
	pipelineInfo.stagesDescription[0].stage = ShaderStage::Vertex;
	pipelineInfo.stagesDescription[0].path = "C:\\dev\\pet\\whatever\\whatever\\source\\shaders\\testgraphics.hlsl";
	pipelineInfo.stagesDescription[0].entryPoint = "VS";
	pipelineInfo.stagesDescription[1].stage = ShaderStage::Fragment;
	pipelineInfo.stagesDescription[1].path = "C:\\dev\\pet\\whatever\\whatever\\source\\shaders\\testgraphics.hlsl";
	pipelineInfo.stagesDescription[1].entryPoint = "PS";
	//pipelineInfo.vertexBufferLayout
	
	pipelineInfo.vertexTopology = PrimitiveTopology::TriangleList;

	ViewportInfo viewport;
	viewport.x = 0;
	viewport.y = 0;
	viewport.width = m_windowSize.x;
	viewport.height = m_windowSize.y;
	pipelineInfo.viewportInfo = viewport;

	auto graphPipeline = m_graphicsEngine->CreateGraphicsPipeline(pipelineInfo);

	IFramebuffer::CreateInfo framebufferInfo{};
	IImage::View swapchainImageView(m_graphicsEngine->GetBackbuffer().get());
	framebufferInfo.colorBuffers = { swapchainImageView };
	framebufferInfo.layout = framebufferLayout;
	auto framebuffer = graphPipeline->CreateFramebuffer(framebufferInfo);



	while (m_window->IsOpen())
	{
		m_graphicsEngine->BeginFrame();
		auto commandBuffer = m_graphicsEngine->CreateCommandBuffer();
		glm::vec4 clearColor(0, 1, 0, 1);

		commandBuffer->Begin();
		commandBuffer->SetClearColorValue(0, &clearColor.x);
		commandBuffer->SetViewport(viewport);
		commandBuffer->SetScissor(Rect2D{ 0, 0, m_windowSize.x, m_windowSize.y });
		commandBuffer->BindPipelineAndFramebuffer(graphPipeline.get(), framebuffer.get());
		commandBuffer->End();

		m_graphicsEngine->Submit(commandBuffer.get());
		m_graphicsEngine->Present();
		m_window->Update();

		//commandBuffer->Reset();
	}
	int a = -0;
}
