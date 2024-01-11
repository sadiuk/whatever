#pragma once
#include "memory"
#include "graphics/IEngine.h"
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
	std::string m_name = "Test";
};