#pragma once
#include <memory>
#include <string>
#include "ui/IWindow.h"
#include "ISwapChain.h"
#include "IGraphicsPipeline.h"
#define ENGINE_VERSION 0



struct IEngine
{
	IEngine(IServiceProvider* services) : m_services(services) {}
	enum GraphicsAPI
	{
		Vulkan
	};
	struct CreationParams
	{
		GraphicsAPI api;
		std::string appName;
		std::shared_ptr<IWindow> window;
		ISwapChain::CreateInfo swapchainInfo;
	};
	static std::shared_ptr<IEngine> Create(const CreationParams& params, IServiceProvider* services);
	virtual GraphicsAPI GetAPI() = 0;

	virtual std::shared_ptr<IGraphicsPipeline> CreateGraphicsPipeline(const IGraphicsPipeline::CreateInfo& params) = 0;
	virtual ImageFormat GetSwapchainFormat() = 0;



private:
	IServiceProvider* m_services,
};
