#pragma once
#include "ICommandBuffer.h"
#include "IFence.h"
#include "ISemaphore.h"
#include "IDevice.h"
#include "util/RefPtr.h"

namespace wtv
{
	struct QueueSubmitInfo
	{
		QueueSubmitInfo(ICommandBuffer* cmdBuf, IDevice* device)
		{
			m_commandBuffers.push_back(cmdBuf);
			m_signalSemaphore = device->CreateBinarySemaphore();
		}
		QueueSubmitInfo(ICommandBuffer** buffers, uint32_t buffersCount, 
			const RefPtr<IBinarySemaphore> signalSem, IBinarySemaphore* waitSemaphore = nullptr) :
			m_commandBuffers(buffers, buffers + buffersCount), m_signalSemaphore(signalSem), m_waitSemaphore(waitSemaphore) {}
		const RefPtr<IBinarySemaphore>& GetSignalSemaphore() const { return m_signalSemaphore; }
		const std::vector<ICommandBuffer*>& GetCommandBuffers() const { return m_commandBuffers; }
	private:
		std::vector<ICommandBuffer*> m_commandBuffers;
		RefPtr<IBinarySemaphore> m_signalSemaphore;
		IBinarySemaphore* m_waitSemaphore;
	};

	struct IQueue : public IReferenceCounted
	{
		virtual void Submit(const QueueSubmitInfo& submitInfo) = 0;

		virtual IFence& GetFence() = 0;
	};
}