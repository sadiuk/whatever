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

		std::vector<VkDescriptorBindingFlags> bindingFlags(params.m_entries.size());
		std::for_each(bindingFlags.begin(), bindingFlags.end(), [](VkDescriptorBindingFlags& fl) {
			fl = VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT | VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT; });
		

		VkDescriptorSetLayoutBindingFlagsCreateInfo flagsInfo{};
		flagsInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
		flagsInfo.bindingCount = params.m_entries.size();
		flagsInfo.pBindingFlags = bindingFlags.data();

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.pNext = params.m_bindless ? &flagsInfo : nullptr;
		layoutInfo.flags = params.m_bindless ? VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT : 0;
		layoutInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
		layoutInfo.pBindings = layoutBindings.data();


		vkCreateDescriptorSetLayout(m_device->GetDevice(), &layoutInfo, nullptr, &m_layout);
	}

	VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout()
	{
		vkDestroyDescriptorSetLayout(m_device->GetDevice(), m_layout, nullptr);
	}


	VulkanDescriptorSet::VulkanDescriptorSet(VulkanDevice* device, const RefPtr<VulkanDescriptorPool>& descPool, RefPtr<VulkanDescriptorSetLayout>& layout)
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

		m_resources.reserve(32);
	}

	VulkanDescriptorSet::~VulkanDescriptorSet()
	{
		uint64_t semaphoreWaitValue = GetSemaphoreWaitValue();
		VkDevice device = m_device->GetDevice();
		m_device->EnqueueForDeletion([semaphoreWaitValue, descPool = m_descPool](uint64_t completedValue)
			{
				if (completedValue >= semaphoreWaitValue)
				{
					// Release the descPool reference
					return true;
				}
				return false;
			});

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
			.dstBinding = entry->slot,
			.dstArrayElement = slot - entry->slot,
			.descriptorCount = 1,
			.descriptorType = VulkanConstantTranslator::GetVkDescriptorType(entry->type),
			.pBufferInfo = &bufferInfo
		};

		vkUpdateDescriptorSets(m_device->GetDevice(), 1, &write, 0, NULL);
		m_resources.push_back(std::pair<uint32_t, IGPUMemoryResource*>(slot, buffer));
	}

	void VulkanDescriptorSet::SetBinding(uint32_t slot, int sampler)
	{
	auto params = m_layout->GetParams();
		const auto* entry = params.GetEntry(slot);
		assert(entry);
		if (!entry)
			return;

		VkDescriptorImageInfo imageInfo = {
			.sampler = m_device->GetSampler(sampler),
		};

		VkWriteDescriptorSet write = {
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.dstSet = m_descriptorSet,
			.dstBinding = entry->slot,
			.dstArrayElement = slot - entry->slot,
			.descriptorCount = 1,
			.descriptorType = VulkanConstantTranslator::GetVkDescriptorType(entry->type),
			.pImageInfo = &imageInfo
		};

		vkUpdateDescriptorSets(m_device->GetDevice(), 1, &write, 0, NULL);
	}

	void VulkanDescriptorSet::SetBinding(uint32_t slot, const IImage::View& image, ImageLayout layout, int sampler)
	{
		auto params = m_layout->GetParams();
		const auto* entry = params.GetEntry(slot);
		assert(entry);
		if (!entry)
			return;

		VkDescriptorImageInfo imageInfo = {
			.imageView = static_cast<VulkanGPUImage*>(image.image)->GetImageView(image),
			.imageLayout = VulkanConstantTranslator::GetVkImageLayout(layout)
		};

		VkWriteDescriptorSet write = {
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.dstSet = m_descriptorSet,
			.dstBinding = entry->slot,
			.dstArrayElement = slot - entry->slot,
			.descriptorCount = 1,
			.descriptorType = VulkanConstantTranslator::GetVkDescriptorType(entry->type),
			.pImageInfo = &imageInfo
		};

		vkUpdateDescriptorSets(m_device->GetDevice(), 1, &write, 0, NULL);
		m_resources.push_back(std::pair<uint32_t, IGPUMemoryResource*>(slot, static_cast<IGPUMemoryResource*>(static_cast<VulkanGPUImage*>(image.image))));
	}
}