#include "VulkanQueue.h"
#include "VulkanCommandBuffer.h"
#include "VulkanDevice.h"

namespace wtv
{
	wtv::VulkanQueue::VulkanQueue(VulkanDevice* engine, uint32_t queueFamilyIndex, uint32_t queueIndex) :
		m_engine(engine), m_device(engine->GetDevice()), m_signalSemaphore(engine), m_fence(engine, true)
	{
		vkGetDeviceQueue(m_device, queueFamilyIndex, queueIndex, &m_queue);
	}

	void wtv::VulkanQueue::Submit(ICommandBuffer* cmdBuffer)
	{
		m_signalSemaphore = VulkanSemaphore(m_engine);
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
		m_fence.Reset();
		vkQueueSubmit(m_queue, 1, &submitInfo, m_fence.GetNativeHandle());
	}

	void wtv::VulkanQueue::Submit(ICommandBuffer* cmdBuffer, IQueue* waitQueue)
	{
		VkPipelineStageFlags waitFlags = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
		VkSemaphore waitSemaphore = static_cast<VulkanQueue*>(waitQueue)->GetRenderFinishedSemaphore().GetNativeHandle();
		VulkanCommandBuffer* vulkanCb = static_cast<VulkanCommandBuffer*>(cmdBuffer);
		VkCommandBuffer rawCb = vulkanCb->GetNativeHandle();
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.pNext = nullptr;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &rawCb;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &waitSemaphore;
		submitInfo.pWaitDstStageMask = &waitFlags;
		submitInfo.signalSemaphoreCount = 1;
		VkSemaphore rawSignalSem = m_signalSemaphore.GetNativeHandle();
		submitInfo.pSignalSemaphores = &rawSignalSem;

		vkQueueSubmit(m_queue, 1, &submitInfo, m_fence.GetNativeHandle());
	}

	IFence& wtv::VulkanQueue::GetFence()
	{
		return m_fence;
	}

	void wtv::VulkanPresentQueue::PresentSwapchainImage(VulkanSwapchain* swapchain, VulkanQueue* waitQueue)
	{
		VkResult result{};
		uint32_t imageIndex = swapchain->GetImageIndex();
		VkSemaphore waitSemaphore = waitQueue->GetRenderFinishedSemaphore().GetNativeHandle();
		VkSwapchainKHR swapchains[] = { swapchain->GetNativeHandle() };
		VkPresentInfoKHR presentInfo{};

		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.pNext = nullptr;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &waitSemaphore;
		presentInfo.pResults = &result;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapchains;

		vkQueuePresentKHR(m_queue, &presentInfo);
		int a = 0;
	}
}