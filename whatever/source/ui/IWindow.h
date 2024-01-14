#pragma once
#include <string>




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
	uint32_t width;
	uint32_t height;
	uint32_t posX;
	uint32_t posY;
};
	virtual bool IsOpen() = 0;
	virtual void* GetNativeHandle() = 0;
	virtual WINDOW_LIB GetWindowingLib() = 0;
};