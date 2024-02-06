#pragma once
#include <memory>
#include "ui/IWindow.h"
#include "IEngine.h"

namespace wtv
{

	struct ISurface
	{
		struct CreationParams
		{
			std::shared_ptr<IWindow> window;
		};

		virtual void* GetNativeHandle() = 0;
	};

	struct ISurfaceFactory
	{
		ISurfaceFactory(const ISurface::CreationParams& params) : m_params(params) {}
		virtual std::shared_ptr<ISurface> Create() = 0;
		virtual ~ISurfaceFactory() = default;
	protected:
		ISurface::CreationParams m_params;
	};
}