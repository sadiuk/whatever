#include "VMAAllocator.h"
#include "VulkanDevice.h"

namespace wtv
{
	VMAAllocator::VMAAllocator(VulkanDevice* device, IServiceProvider* services) :
		m_device(device),
		m_services(services)
	{
		VmaAllocatorCreateInfo allocatorCreateInfo = {};
		allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_2; // Ваша версія API
		allocatorCreateInfo.physicalDevice = m_device->GetPhysicalDevice();
		allocatorCreateInfo.device = m_device->GetDevice();
		allocatorCreateInfo.instance = m_device->GetInstance();

		ASSERT_VK_SUCCESS(vmaCreateAllocator(&allocatorCreateInfo, &m_allocator));

	}

	VMAAllocator::~VMAAllocator()
	{
		vmaDestroyAllocator(m_allocator);
	}

	VkBuffer VMAAllocator::AllocateBuffer(const IGPUBuffer::CreationParams& params)
	{
		auto device = m_device->GetDevice();
		auto physDevice = m_device->GetPhysicalDevice();


		auto indices = m_device->GetQueueFamilyIndices().Vector();
		VkBufferCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.size = params.bufferSize;
		createInfo.pQueueFamilyIndices = indices.data();
		createInfo.queueFamilyIndexCount = (uint32_t)indices.size();
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.usage = VulkanConstantTranslator::GetVkBufferUsageFlags((uint32_t)params.usageFlags);
		//TODO: ditch this, must be done manually
		createInfo.usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		createInfo.usage |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

		VmaAllocationCreateInfo allocCreateInfo = {};
		allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
		if ((params.memoryFlags & MemoryPropertyFlags::HostVisible) == MemoryPropertyFlags::HostVisible)
			allocCreateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
		else
			allocCreateInfo.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
		allocCreateInfo.priority = 1.0f;

		VkBuffer buffer;
		VmaAllocation allocation;
		ASSERT_VK_SUCCESS_ELSE_RET0(vmaCreateBuffer(m_allocator, &createInfo, &allocCreateInfo, &buffer, &allocation, nullptr));
		
		ASSERT_TRUE_ELSE_RET0(m_bufferAllocationsMap.find(buffer) == m_bufferAllocationsMap.end());
		m_bufferAllocationsMap[buffer] = allocation;
		return buffer;
	}

	VkImage VMAAllocator::AllocateImage(const IImage::CreationParams& createInfo, MemoryPropertyFlags memoryFlags)
	{
		auto device = m_device->GetDevice();
		auto physDevice = m_device->GetPhysicalDevice();
		auto indices = m_device->GetQueueFamilyIndices().Vector();

		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.pNext = nullptr;
		imageInfo.imageType = VulkanConstantTranslator::GetVkImageType(createInfo.dimension);
		imageInfo.format = VulkanConstantTranslator::GetVkFormat(createInfo.format);
		imageInfo.extent.width = createInfo.width;
		imageInfo.extent.height = createInfo.height;
		imageInfo.extent.depth = createInfo.depth;
		imageInfo.mipLevels = createInfo.mipLevels;
		imageInfo.arrayLayers = createInfo.arraySize;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.usage = VulkanConstantTranslator::GetVkImageUsageMask((uint32_t)createInfo.usageFlags);
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.queueFamilyIndexCount = 0;
		imageInfo.pQueueFamilyIndices = nullptr;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		VmaAllocationCreateInfo allocCreateInfo = {};
		allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
		if ((memoryFlags & MemoryPropertyFlags::HostVisible) == MemoryPropertyFlags::HostVisible)
			allocCreateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
		else
			allocCreateInfo.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
		allocCreateInfo.priority = 1.0f;

		VkImage image;
		VmaAllocation allocation;
		ASSERT_VK_SUCCESS_ELSE_RET0(vmaCreateImage(m_allocator, &imageInfo, &allocCreateInfo, &image, &allocation, nullptr));

		ASSERT_TRUE_ELSE_RET0(m_imageAllocationsMap.find(image) == m_imageAllocationsMap.end());
		m_imageAllocationsMap[image] = allocation;
		return image;
	}

	void VMAAllocator::DeallocateBuffer(VkBuffer buffer)
	{
		ASSERT_TRUE_ELSE_RET(m_bufferAllocationsMap.find(buffer) != m_bufferAllocationsMap.end());
		VmaAllocation allocation = m_bufferAllocationsMap[buffer];

		vmaDestroyBuffer(m_allocator, buffer, allocation);

		m_bufferAllocationsMap.erase(buffer);
	}

	void VMAAllocator::DeallocateImage(VkImage image)
	{
		ASSERT_TRUE_ELSE_RET(m_imageAllocationsMap.find(image) != m_imageAllocationsMap.end())
		VmaAllocation allocation = m_imageAllocationsMap[image];

		vmaDestroyImage(m_allocator, image, allocation);
		m_imageAllocationsMap.erase(image);

	}

	void* VMAAllocator::Map(VkBuffer buffer)
	{
		void* data;
		VmaAllocation allocation = m_bufferAllocationsMap[buffer];
		vmaMapMemory(m_allocator, allocation, &data);
		return data;
	}

	void VMAAllocator::Unmap(VkBuffer buffer)
	{
		VmaAllocation allocation = m_bufferAllocationsMap[buffer];
		vmaUnmapMemory(m_allocator, allocation);
	}
}
