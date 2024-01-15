#pragma once
#include <string>

#include "glm/glm.hpp"



class IWindow
{
public:
enum WINDOW_LIB
{
	SDL2
};
struct CreationParams
{
	std::string caption;
	glm::uvec2 size;
	glm::ivec2 windowPos;
};
	virtual bool IsOpen() = 0;
	virtual void* GetNativeHandle() = 0;
	virtual WINDOW_LIB GetWindowingLib() = 0;
};