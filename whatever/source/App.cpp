#include "App.h"
#include "vulkan/vulkan.h"

void App::Init()
{
	m_graphicsEngine = IEngine::Create(GRAPHICS_API::VULKAN, m_name);
}
