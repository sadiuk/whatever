#pragma once
class IWindow
{
public:
	virtual bool IsOpen() = 0;
	virtual void* GetNativeHandle() = 0;
};