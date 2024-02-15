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

	ISwapChain::CreateInfo swapchainParams{};
	swapchainParams.extent = m_windowSize;
	IEngine::CreationParams engineParams{};
	engineParams.api = IEngine::GraphicsAPI::Vulkan;
	engineParams.appName = "EngineTest";
	engineParams.window = m_window;
	engineParams.swapchainInfo = swapchainParams;
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

	pipelineInfo.renderTargetInfoVec.resize(1);
	pipelineInfo.renderTargetInfoVec[0].clearBeforeWrite = true;
	pipelineInfo.renderTargetInfoVec[0].format = m_graphicsEngine->GetSwapchainFormat();

	//pipelineInfo.stagesDescription.resize(2);

	//pipelineInfo.vertexBufferLayout
	
	pipelineInfo.vertexTopology = PrimitiveTopology::TriangleList;

	pipelineInfo.viewportInfo.x = 0;
	pipelineInfo.viewportInfo.y = 0;
	pipelineInfo.viewportInfo.width = m_windowSize.x;
	pipelineInfo.viewportInfo.height = m_windowSize.y;

	auto graphPipeline = m_graphicsEngine->CreateGraphicsPipeline(pipelineInfo);

}
