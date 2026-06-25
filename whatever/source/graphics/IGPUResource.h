#pragma once

namespace wtv
{
	enum class GPUResourceType
	{
		Buffer = 1,
		Image,
		Unknown = 0xFFFFFFFF
	};

	struct IGPUMemoryResource
	{
		virtual ~IGPUMemoryResource() = default;
		virtual GPUResourceType GetResourceType() const = 0;
	};

	struct IGPUResource
	{
		virtual ~IGPUResource() = default;
		void SetSemaphoreWaitValue(uint64_t value) { m_waitValue = value; }
		uint64_t GetSemaphoreWaitValue() const { return m_waitValue; }
	private:
		uint64_t m_waitValue = 0;
	};
}