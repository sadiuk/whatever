#pragma once
#include "graphics/IDescriptorSet.h"
#include "graphics/IGPUResource.h"
#include "vulkan/vulkan.h"
#include <vector>
namespace wtv
{
	class VulkanDevice;
	class VulkanDescriptorSetLayout : public IDescriptorSetLayout
	{
		VulkanDevice* m_device;
		VkDescriptorSetLayout m_layout;
		DescriptorSetLayoutParams m_params;
	public:
		VulkanDescriptorSetLayout(VulkanDevice* device, const DescriptorSetLayoutParams& params);
		~VulkanDescriptorSetLayout();
		VkDescriptorSetLayout GetNativeHandle() const { return m_layout; }
		const DescriptorSetLayoutParams& GetParams() const { return m_params; }
	};

	class VulkanDescriptorPool;
	class VulkanDescriptorSet : public IDescriptorSet
	{
		VkDescriptorSet m_descriptorSet;
		VulkanDevice* m_device;
		VulkanDescriptorPool* m_descPool;
		RefPtr<VulkanDescriptorSetLayout> m_layout;
		std::vector<std::pair<uint32_t, IGPUResource*>> m_resources;
	public:
		VulkanDescriptorSet(VulkanDevice* device, VulkanDescriptorPool* descPool, RefPtr<VulkanDescriptorSetLayout>& layout);
		~VulkanDescriptorSet() = default;
		VkDescriptorSet GetNativeHandle() const { return m_descriptorSet; }
		std::vector<std::pair<uint32_t, IGPUResource*>>& GetBoundResources() { return m_resources; }
		IDescriptorSetLayout* GetLayout() override { return m_layout.get(); }
		//TODO:: ensure when adding texture bindings that we also add them to m_resources
		void SetBinding(uint32_t slot, IGPUBuffer* buffer, uint32_t offset, uint32_t size) override;
		void SetBinding(uint32_t slot, int sampler) override;
		void SetBinding(uint32_t slot, const IImage::View& image, ImageLayout layout, int sampler) override;
	};
}