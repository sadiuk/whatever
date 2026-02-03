#pragma once
#include "util/RefPtr.h"
#include "GraphicsConstants.h"
#include "IFramebuffer.h"

#include <vector>


namespace wtv
{
	struct IGPURenderPass : public IReferenceCounted
	{
		virtual ~IGPURenderPass() {}
	};
}