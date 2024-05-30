#include "VulkanGPUBuffer.h"

namespace wtv
{
	VulkanGPUBuffer::VulkanGPUBuffer(VulkanEngine* engine, const CreationParams& params) : IGPUBuffer(params), m_engine(engine)
	{
		auto device = engine->GetDevice();
		auto physDevice = engine->GetPhysicalDevice();


		auto indices = engine->GetQueueFamilyIndices().Vector();
		VkBufferCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.size = params.bufferSize;
		createInfo.pQueueFamilyIndices = indices.data();
		createInfo.queueFamilyIndexCount = (uint32_t)indices.size();
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.usage = VulkanConstantTranslator::GetVkBufferUsageFlags(params.usageFlags);
		createInfo.usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		createInfo.usage |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		ASSERT_VK_SUCCESS(vkCreateBuffer(device, &createInfo, nullptr, &m_buffer));


		VkMemoryRequirements memRequirements{};
		vkGetBufferMemoryRequirements(device, m_buffer, &memRequirements);

		
		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.pNext = nullptr;
		allocInfo.allocationSize = params.bufferSize;
		allocInfo.memoryTypeIndex = m_engine->FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		ASSERT_VK_SUCCESS(vkAllocateMemory(device, &allocInfo, nullptr, &m_memory));
		
		ASSERT_VK_SUCCESS(vkBindBufferMemory(device, m_buffer, m_memory, 0));
	}
	VulkanGPUBuffer::~VulkanGPUBuffer()
	{
	}
}