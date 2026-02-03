#pragma once
#include "util/RefPtr.h"
#include <array>
namespace wtv
{
	struct DescriptorPoolParams
	{
		int maxSetCount;
		std::array<int, (int)DescriptorType::Count> descCounts{};
		void SetDescriptorCount(DescriptorType type, int count)
		{
			descCounts[(int)type] = count;
		}
	};

	struct IDescriptorPool : public IReferenceCounted
	{

	};
}