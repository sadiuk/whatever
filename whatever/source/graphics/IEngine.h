#pragma once
#include <memory>
#include <string>
#define ENGINE_VERSION 0
enum GRAPHICS_API
{
	VULKAN
};

class IEngine
{
public:
	static std::shared_ptr<IEngine> Create(GRAPHICS_API api, const std::string& appName);
};
