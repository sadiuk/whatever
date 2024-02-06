#pragma once
#include "graphics/IEngine.h"
#include "ui/IWindow.h"
#include "IServiceProvider.h"

#include <memory>
#include <string>
class App
{
public:
	App();
	void Run();
private:
	void Init();



	std::shared_ptr<wtv::IEngine> m_graphicsEngine;
	std::shared_ptr<wtv::IWindow> m_window;
	std::shared_ptr<wtv::IServiceProvider> m_services;
	glm::uvec2 m_windowSize;
	glm::uvec2 m_windowPos;
};