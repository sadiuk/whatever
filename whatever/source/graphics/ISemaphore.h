#pragma once
#include "../util/RefPtr.h"

namespace wtv
{
	struct IBinarySemaphore : public IReferenceCounted
	{

	};

	struct ISemaphore : public IReferenceCounted
	{
		virtual uint64_t IncrementValue() = 0;
		virtual uint64_t CheckCompletedValue() const = 0;
		virtual ~ISemaphore() = default;
	};
}