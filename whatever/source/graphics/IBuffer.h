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
			std::underlying_type_t<BufferUsage> usageFlags;
			std::string name;
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

		IBuffer(const CreationParams& params) : m_params(params) {}
		virtual ~IBuffer() = default;
		const CreationParams& GetProperties() const { return m_params; };
	private:
		CreationParams m_params;

	};
}