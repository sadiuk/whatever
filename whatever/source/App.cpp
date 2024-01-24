#include "App.h"
#include "ui/WindowSDL2.h"
#include "vulkan/vulkan.h"
#include "graphics/ISurface.h"
void App::Init()
{
	glm::uvec2 windowSize = { 800, 600 };
	IWindow::CreationParams windowParams{};
	windowParams.caption = "EngineTest";
	windowParams.windowPos.x = 300;
	windowParams.windowPos.y = 200;
	windowParams.size = windowSize;
	m_window = std::make_shared<WindowSDL2>(windowParams);

	ISwapChain::CreateInfo swapchainParams{};
	swapchainParams.extent = windowSize;
	IEngine::CreationParams engineParams{};
	engineParams.api = IEngine::GraphicsAPI::Vulkan;
	engineParams.appName = "EngineTest";
	engineParams.window = m_window;
	engineParams.swapchainInfo = swapchainParams;
	m_graphicsEngine = IEngine::Create(engineParams);
}
