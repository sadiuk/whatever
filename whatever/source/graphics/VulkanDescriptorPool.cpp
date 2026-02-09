#include "VulkanDescriptorPool.h"
#include "VulkanDescriptorSet.h"
#include "IDescriptorSet.h"

namespace wtv
{
	RefPtr<IDescriptorSet> VulkanDescriptorPool::AllocateDescriptorSet(const RefPtr<IDescriptorSetLayout>& layout)
	{
		RefPtr<const VulkanDescriptorSetLayout> vkLayout(static_cast<const VulkanDescriptorSetLayout*>(layout.get()));
		return MakeRef<VulkanDescriptorSet>(m_device, this, vkLayout);
	}
}
