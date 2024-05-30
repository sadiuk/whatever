#pragma once
#include "ICommandBuffer.h"
#include "IFence.h"
#include "util/RefPtr.h"

namespace wtv
{
	struct IQueue : public IReferenceCounted
	{
		virtual void Submit(ICommandBuffer* cmdBuffer) = 0;
		virtual void Submit(ICommandBuffer* cmdBuffer, IQueue* waitQueue) = 0;

		virtual IFence& GetFence() = 0;
	};
}