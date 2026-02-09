#pragma once
#include "util/RefPtr.h"
#include "GraphicsConstants.h"

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

	struct IDescriptorSet;
	struct IDescriptorSetLayout;
	struct IDescriptorPool : public IReferenceCounted
	{
		virtual RefPtr<IDescriptorSet> AllocateDescriptorSet(const RefPtr<IDescriptorSetLayout>& layout) = 0;
		virtual ~IDescriptorPool() {}
	};
}