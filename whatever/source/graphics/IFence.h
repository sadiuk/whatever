#pragma once
#include "util/RefPtr.h"

namespace wtv
{
	struct IFence : public IReferenceCounted
	{
		virtual void Wait() const = 0;
		virtual void Reset() = 0;
	};
}
	