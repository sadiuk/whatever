#include "App.h"
#include "ui/WindowSDL2.h"
#include "vulkan/vulkan.h"
#include "graphics/ISurface.h"
void App::Init()
{
	IWindow::CreationParams windowParams{};
	windowParams.caption = "EngineTest";
	windowParams.posX = 300;
	windowParams.posY = 200;
	windowParams.width = 800;
	windowParams.height = 600;
	m_window = std::make_shared<WindowSDL2>(windowParams);

	IEngine::CreationParams engineParams{};
	engineParams.api = IEngine::GRAPHICS_API::VULKAN;
	engineParams.appName = "EngineTest";
	engineParams.window = m_window;
	m_graphicsEngine = IEngine::Create(engineParams);
}
