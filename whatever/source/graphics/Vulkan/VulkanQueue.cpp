#include "VulkanQueue.h"
#include "VulkanCommandBuffer.h"
#include "VulkanDevice.h"
#include "VulkanSync.h"

namespace wtv
{
	wtv::VulkanQueue::VulkanQueue(VulkanDevice* engine, uint32_t queueFamilyIndex, uint32_t queueIndex) :
		m_device(engine), m_deviceRaw(engine->GetDevice()), m_fence(engine, true),
		m_queueFamilyIndex(queueFamilyIndex)
	{
		vkGetDeviceQueue(m_deviceRaw, queueFamilyIndex, queueIndex, &m_queue);
	}

	void VulkanQueue::Submit(const QueueSubmitInfo& submitInfo)
	{
		auto& cmdBuffers = submitInfo.GetCommandBuffers();


		m_signalSemaphore = StaticRefCast<VulkanBinarySemaphore>(submitInfo.GetSignalSemaphore());
		VulkanSemaphore& globalSemaphore = m_device->GetTimelineSemaphore();
		uint64_t nextTimelineValue = globalSemaphore.IncrementValue();

		std::vector<VkCommandBuffer> rawCbs(cmdBuffers.size());
		for (int i = 0; i < rawCbs.size(); i++)
		{
			rawCbs[i] = static_cast<VulkanCommandBuffer*>(cmdBuffers[i])->GetNativeHandle();
		}

		uint64_t signalSemaphoreValues[] = { 0, nextTimelineValue };
		VkTimelineSemaphoreSubmitInfo timelineInfo = {};
		timelineInfo.sType = VK_STRUCTURE_TYPE_TIMELINE_SEMAPHORE_SUBMIT_INFO;
		timelineInfo.signalSemaphoreValueCount = 2;
		timelineInfo.pSignalSemaphoreValues = signalSemaphoreValues;

		VkSubmitInfo vkSubmitInfo{};
		vkSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		vkSubmitInfo.pNext = &timelineInfo;
		vkSubmitInfo.commandBufferCount = rawCbs.size();
		vkSubmitInfo.pCommandBuffers = rawCbs.data();
		vkSubmitInfo.waitSemaphoreCount = 0;
		vkSubmitInfo.pWaitSemaphores = nullptr;
		vkSubmitInfo.pWaitDstStageMask = nullptr;
		vkSubmitInfo.signalSemaphoreCount = 2;
		VkSemaphore rawSignalSems[] = { m_signalSemaphore->GetNativeHandle(), globalSemaphore.GetNativeHandle() };
		vkSubmitInfo.pSignalSemaphores = rawSignalSems;
		m_fence.Reset();
		vkQueueSubmit(m_queue, 1, &vkSubmitInfo, m_fence.GetNativeHandle());

		for (const auto& cb : cmdBuffers)
		{
			auto vulkanCB = static_cast<VulkanCommandBuffer*>(cb);
			for (auto& res : vulkanCB->ListResourcesInUse())
			{
				res->SetSemaphoreWaitValue(nextTimelineValue);
			}
		}
		m_device->FlushDeletions();
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