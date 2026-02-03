#pragma once
#include "IDescriptorPool.h"
#include "VulkanConstantTranslator.h"
#include "VulkanDevice.h"

#include "vulkan/vulkan.h"
namespace wtv
{
	class VulkanDescriptorPool : public IDescriptorPool
	{
		VkDescriptorPool m_descPool;
		DescriptorPoolParams m_params;
		VulkanDevice* m_device;
	public:
		VulkanDescriptorPool(VulkanDevice* device, const DescriptorPoolParams& params) :
			m_params(params),
			m_device(device)
		{
			VkDescriptorPoolCreateInfo info{};
			info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			info.maxSets = params.maxSetCount;
			std::vector<VkDescriptorPoolSize> sizes;
			ObtainPoolSizes(sizes);
			info.poolSizeCount = sizes.size();
			info.pPoolSizes = sizes.data();
			vkCreateDescriptorPool(m_device->GetDevice(), &info, nullptr, &m_descPool);
		}

		~VulkanDescriptorPool()
		{
			vkDestroyDescriptorPool(m_device->GetDevice(), m_descPool, nullptr);
		}

		VkDescriptorPool GetNativeHandle() const { return m_descPool; }
	private:
		void ObtainPoolSizes(std::vector<VkDescriptorPoolSize>& sizes)
		{
			for (int i = 0; i < m_params.descCounts.size(); i++)
			{
				if (m_params.descCounts[i] > 0)
				{
					VkDescriptorPoolSize size;
					size.type = VulkanConstantTranslator::GetVkDescriptorType((DescriptorType)i);
					size.descriptorCount = m_params.descCounts[i];
					sizes.emplace_back(size);
				}
			}
		}
	};
}