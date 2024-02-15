#pragma once
#include "ui/IWindow.h"
#include "IEngine.h"
#include <util/RefPtr.h>

#include <memory>
namespace wtv
{

	struct ISurface : public IReferenceCounted
	{
		struct CreationParams
		{
			RefPtr<IWindow> window;
		};

		virtual void* GetNativeHandle() = 0;
	};

	struct ISurfaceFactory
	{
		ISurfaceFactory(const ISurface::CreationParams& params) : m_params(params) {}
		virtual RefPtr<ISurface> Create() = 0;
		virtual ~ISurfaceFactory() = default;
	protected:
		ISurface::CreationParams m_params;
	};
}