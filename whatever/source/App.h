#pragma once
#include "memory"
#include "graphics/IEngine.h"
#include "ui/IWindow.h"
#include <string>
class App
{
public:
	App()
	{
		Init();
	}
private:
	void Init();


	std::shared_ptr<IEngine> m_graphicsEngine;
	std::shared_ptr<IWindow> m_window;
};