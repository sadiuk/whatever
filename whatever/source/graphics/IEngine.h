#pragma once
#include <memory>
#include <string>
#include "ui/IWindow.h"
#include "ISwapChain.h"
#include "IGraphicsPipeline.h"
#include "ICommandBuffer.h"
#include "IServiceProvider.h"
#include "util/RefPtr.h"
#define ENGINE_VERSION 0


namespace wtv
{
	struct IEngine : public IReferenceCounted
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
			RefPtr<IWindow> window;
		};
		static RefPtr<IEngine> Create(const CreationParams& params, IServiceProvider* services);
		virtual GraphicsAPI GetAPI() = 0;

		virtual RefPtr<IGraphicsPipeline> CreateGraphicsPipeline(const IGraphicsPipeline::CreateInfo& params) = 0;
		virtual RefPtr<ICommandBuffer> CreateCommandBuffer() = 0;
		virtual RefPtr<IGPUImage> GetBackbuffer() = 0;
		virtual ImageFormat GetSwapchainFormat() = 0;
		virtual void BeginFrame() = 0;
		virtual void Submit(ICommandBuffer* cb) = 0;
		virtual void Present() = 0;
		virtual ~IEngine() {}
	protected:
		IServiceProvider* m_services;
	};
}