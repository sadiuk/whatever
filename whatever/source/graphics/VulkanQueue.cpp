#include "VulkanQueue.h"
#include "VulkanCommandBuffer.h"
#include "VulkanEngine.h"

wtv::VulkanQueue::VulkanQueue(VulkanEngine* engine, uint32_t queueFamilyIndex, uint32_t queueIndex):
	m_engine(engine), m_device(engine->GetDevice()), m_signalSemaphore(engine), m_fence(engine)
{
	vkGetDeviceQueue(m_device, queueFamilyIndex, queueIndex, &m_queue);	
}

void wtv::VulkanQueue::Submit(ICommandBuffer* cmdBuffer)
{
	VulkanCommandBuffer* vulkanCb = static_cast<VulkanCommandBuffer*>(cmdBuffer);
	VkCommandBuffer rawCb = vulkanCb->GetNativeHandle();
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = nullptr;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &rawCb;
	submitInfo.waitSemaphoreCount = 0;
	submitInfo.pWaitSemaphores = nullptr;
	submitInfo.pWaitDstStageMask = nullptr;
	submitInfo.signalSemaphoreCount = 1;
	VkSemaphore rawSignalSem = m_signalSemaphore.GetNativeHandle();
	submitInfo.pSignalSemaphores = &rawSignalSem;

	vkQueueSubmit(m_queue, 1, &submitInfo, m_fence.GetNativeHandle());
}
