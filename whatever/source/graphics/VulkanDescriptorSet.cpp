#include "VulkanDescriptorSet.h"
#include "VulkanDevice.h"
#include "VulkanDescriptorPool.h"
#include "VulkanGPUBuffer.h"

namespace wtv
{
	VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(VulkanDevice* device, const DescriptorSetLayoutParams& params) :
		m_device(device),
		m_params(params)
	{
		std::vector<VkDescriptorSetLayoutBinding> layoutBindings(params.m_entries.size());
		for (int i = 0; i < params.m_entries.size(); ++i)
		{
			layoutBindings[i].binding = params.m_entries[i].slot;
			layoutBindings[i].descriptorCount = params.m_entries[i].count;
			layoutBindings[i].descriptorType = VulkanConstantTranslator::GetVkDescriptorType(params.m_entries[i].type);
			layoutBindings[i].stageFlags = (VkShaderStageFlags)VulkanConstantTranslator::GetVkShaderStageFlags(params.m_entries[i].stages);
			layoutBindings[i].pImmutableSamplers = nullptr;
		}
		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.pNext = nullptr;
		layoutInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
		layoutInfo.pBindings = layoutBindings.data();
		vkCreateDescriptorSetLayout(m_device->GetDevice(), &layoutInfo, nullptr, &m_layout);
	}

	VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout()
	{
		vkDestroyDescriptorSetLayout(m_device->GetDevice(), m_layout, nullptr);
	}


	VulkanDescriptorSet::VulkanDescriptorSet(VulkanDevice* device, VulkanDescriptorPool* descPool, RefPtr<const VulkanDescriptorSetLayout>& layout)
		: m_device(device), 
		m_descPool(descPool),
		m_layout(layout)
	{
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.pNext = nullptr;
		allocInfo.descriptorPool = descPool->GetNativeHandle();
		allocInfo.descriptorSetCount = 1;
		VkDescriptorSetLayout vkLayout = m_layout->GetNativeHandle();
		allocInfo.pSetLayouts = &vkLayout;
		vkAllocateDescriptorSets(m_device->GetDevice(), &allocInfo, &m_descriptorSet);
	}

	void VulkanDescriptorSet::SetBinding(uint32_t slot, IGPUBuffer* buffer, uint32_t offset, uint32_t size)
	{
		auto params = m_layout->GetParams();
		const auto* entry = params.GetEntry(slot);
		assert(entry);
		if (!entry)
			return;
		
		VkDescriptorBufferInfo bufferInfo = {
			.buffer = static_cast<VulkanGPUBuffer*>(buffer)->GetNativeHandle(),
			.offset = offset,
			.range = size
		};

		VkWriteDescriptorSet write = {
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.dstSet = m_descriptorSet,
			.dstBinding = slot,
			.descriptorCount = 1,
			.descriptorType = VulkanConstantTranslator::GetVkDescriptorType(entry->type),
			.pBufferInfo = &bufferInfo
		};

		vkUpdateDescriptorSets(m_device->GetDevice(), 1, &write, 0, NULL);

	}
}