#pragma once
#include "graphics/IEngine.h"
#include "ui/IWindow.h"
#include "IServiceProvider.h"
#include "util/RefPtr.h"

#include <memory>
#include <string>
class App
{
public:
	App();
	void Run();
private:
	void Init();



	wtv::RefPtr<wtv::IEngine> m_graphicsEngine;
	wtv::RefPtr<wtv::IWindow> m_window;
	wtv::RefPtr<wtv::IServiceProvider> m_services;
	glm::uvec2 m_windowSize;
	glm::uvec2 m_windowPos;
};