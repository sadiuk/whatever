#include "VulkanDescriptorPool.h"
#include "VulkanDescriptorSet.h"
#include "graphics/IDescriptorSet.h"

namespace wtv
{
	RefPtr<IDescriptorSet> VulkanDescriptorPool::AllocateDescriptorSet(RefPtr<IDescriptorSetLayout>& layout)
	{
		RefPtr<VulkanDescriptorSetLayout> vkLayout(static_cast<VulkanDescriptorSetLayout*>(layout.get()));
		return MakeRef<VulkanDescriptorSet>(m_device, this, vkLayout);
	}
}
