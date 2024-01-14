#pragma once
#include <memory>
#include <string>
#include "ui/IWindow.h"

#define ENGINE_VERSION 0



struct IEngine
{
	enum GRAPHICS_API
	{
		VULKAN
	};
	struct CreationParams
	{
		GRAPHICS_API api;
		std::string appName;
		std::shared_ptr<IWindow> window;
	};
	static std::shared_ptr<IEngine> Create(const CreationParams& params);
	virtual GRAPHICS_API GetAPI() = 0;
};
