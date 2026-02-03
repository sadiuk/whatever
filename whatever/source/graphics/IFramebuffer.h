#pragma once
#include "GraphicsConstants.h"
#include "IGPUImage.h"
#include "util/RefPtr.h"

#include <vector>
#include <optional>
#include <functional>

#include "glm/glm.hpp"

namespace wtv
{
	struct IFramebuffer : public IReferenceCounted
	{
		struct AttachmentInfo
		{
			ImageFormat format;
			AttachmentLoadOp loadOp = AttachmentLoadOp::Load;
			AttachmentStoreOp storeOp = AttachmentStoreOp::Store;
			std::optional<glm::vec4> clearColor;
			bool operator==(const AttachmentInfo&) const = default;
		};
		struct Layout
		{
			std::vector<AttachmentInfo> colorBuffers;
			std::optional<AttachmentInfo> depthBuffer;
			int GetAttachmentCount() const
			{
				int count = (int)colorBuffers.size();
				if (depthBuffer.has_value())
					count += 1;
				return count;
			}
			const AttachmentInfo& GetRTInfo(int index) const
			{
				assert(index >= 0 && index < GetAttachmentCount());
				if (index < colorBuffers.size())
					return colorBuffers[index];
				else
					return depthBuffer.value();
			}

			bool operator==(const Layout&) const = default;
		};
		struct Properties
		{
			Layout layout;
			std::vector<IImage::View> colorBuffers;
			std::optional<IImage::View> depthBuffer;


		};
		IFramebuffer(Properties&& params) : m_params(std::move(params)) {}
		virtual ~IFramebuffer() = default;
		const Properties& GetProperties() const { return m_params; }
	protected:
		Properties m_params;

	};

}

// Hash specialization for std::unordered_map support
namespace std
{
	template<>
	struct hash<wtv::IFramebuffer::AttachmentInfo>
	{
		size_t operator()(const wtv::IFramebuffer::AttachmentInfo& info) const noexcept
		{
			size_t h1 = hash<int>{}(static_cast<int>(info.format));
			size_t h2 = hash<int>{}(static_cast<int>(info.loadOp));
			size_t h3 = hash<int>{}(static_cast<int>(info.storeOp));

			// Combine hashes using a method similar to boost::hash_combine
			size_t seed = 0;
			seed ^= h1 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			seed ^= h2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			seed ^= h3 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			return seed;
		}
	};

	template<>
	struct hash<wtv::IFramebuffer::Layout>
	{
		size_t operator()(const wtv::IFramebuffer::Layout& layout) const noexcept
		{
			size_t seed = 0;

			// Hash the color buffers
			for (const auto& colorBuffer : layout.colorBuffers)
			{
				size_t h = hash<wtv::IFramebuffer::AttachmentInfo>{}(colorBuffer);
				seed ^= h + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			}

			// Hash the depth buffer if present
			if (layout.depthBuffer.has_value())
			{
				size_t h = hash<wtv::IFramebuffer::AttachmentInfo>{}(layout.depthBuffer.value());
				seed ^= h + 0x9e3779b9 + (seed << 6) + (seed >> 2);
				// Also mix in a flag that depth buffer exists
				seed ^= 0x9e3779b9 + (seed << 6) + (seed >> 2);
			}

			return seed;
		}
	};
}

