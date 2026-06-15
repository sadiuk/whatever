#pragma once
#include "util/RefPtr.h"
#include "GraphicsConstants.h"

#include <type_traits>
#include <string>

namespace wtv
{
	struct IBuffer : public IReferenceCounted
	{
		struct CreationParams
		{
			size_t bufferSize;
			size_t alignment = 0;
			BufferUsage usageFlags = BufferUsage::Undefined;
			MemoryPropertyFlags memoryFlags = MemoryPropertyFlags::Undefined;
		};
		struct View
		{
			IBuffer* buffer;
			size_t offset;
			size_t size;
			bool operator==(const View& other) const = default;
			View(IBuffer* buf)
			{
				buffer = buf;
				offset = 0;
				size = buffer->GetProperties().bufferSize;
			}
			View(IBuffer* buf, size_t offset, size_t size)
			{
				buffer = buf;
				offset = offset;
				size = size;
			}
		};

		IBuffer(const CreationParams& params, const std::string& name) : m_params(params), m_name(name) {}
		virtual ~IBuffer() = default;
		const CreationParams& GetProperties() const { return m_params; };
		uint64_t GetSize() const { return m_params.bufferSize; }
	private:
		CreationParams m_params;
		std::string m_name;
	};
}