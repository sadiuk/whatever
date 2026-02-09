#pragma once
#include "GraphicsConstants.h"
#include "util/RefPtr.h"

#include <vector>
namespace wtv
{
	class IGPUBuffer;
	struct DescriptorSetLayoutParams
	{
		struct LayoutEntry
		{
			uint32_t slot;
			DescriptorType type = DescriptorType::Undefined;
			uint32_t count;
			ShaderStageFlags stages;
		};
		std::vector<LayoutEntry> m_entries;

		DescriptorSetLayoutParams(uint32_t bindingCount = 0) : m_entries(bindingCount) {}
		void DescribeLayoutEntry(uint32_t slot, DescriptorType type, uint32_t count, ShaderStageFlags stages)
		{
			m_lastDescribedIndex++;
			m_entries[m_lastDescribedIndex].slot = slot;
			m_entries[m_lastDescribedIndex].type = type;
			m_entries[m_lastDescribedIndex].count = count;
			m_entries[m_lastDescribedIndex].stages = stages;
		}

		const LayoutEntry* GetEntry(uint32_t slot) const
		{
			for (const auto& entry : m_entries)
			{
				if (entry.slot == slot)
					return &entry;
			}
			return nullptr;
		}
	private:
		int32_t m_lastDescribedIndex = -1;
	};

	struct IDescriptorSetLayout : public IReferenceCounted
	{
		virtual ~IDescriptorSetLayout() {}
	};

	struct IDescriptorSet : public IReferenceCounted
	{
		virtual void SetBinding(uint32_t slot, IGPUBuffer* buffer, uint32_t offset, uint32_t size) = 0;
		virtual ~IDescriptorSet() {}
	};
}