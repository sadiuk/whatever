#pragma once
#include <memory>
#include <string>
#include "ui/IWindow.h"
#include "ISwapChain.h"
#include "IGraphicsPipeline.h"
#include "ICommandBuffer.h"
#include "IGPUBuffer.h"
#include "IFence.h"
#include "IGPURenderPass.h"
#include "IServiceProvider.h"
#include "IDescriptorPool.h"
#include "ISemaphore.h"
#include "Sampler.h"
#include "util/RefPtr.h"
#define ENGINE_VERSION 0


namespace wtv
{
	class IQueue;
	using SamplerIndex = int;
	struct IDevice : public IReferenceCounted, IServiceProviderHolder
	{
		IDevice(IServiceProvider* services) : m_services(services) {}
		enum GraphicsAPI
		{
			Vulkan
		};
		struct CreationParams
		{
			GraphicsAPI api;
			std::string appName;
			RefPtr<IWindow> window;
		};
		static RefPtr<IDevice> Create(const CreationParams& params, IServiceProvider* services);
		virtual GraphicsAPI GetAPI() = 0;

		virtual RefPtr<IGraphicsPipelineLayout> CreateGraphicsPipelineLayout(const GraphicsPipelineLayoutCreateInfo& params) = 0;
		virtual RefPtr<IGraphicsPipeline> CreateGraphicsPipeline(const IGraphicsPipeline::CreateInfo& params, const RefPtr<const IGraphicsPipelineLayout>& layout) = 0;
		virtual RefPtr<ICommandBuffer> CreateCommandBuffer() = 0;
		virtual IGPUImage* GetBackbuffer() = 0;
		virtual RefPtr<IFence> CreateFence(bool createSignaled) = 0;
		virtual RefPtr<IGPUBuffer> CreateBuffer(const IGPUBuffer::CreationParams& params, const std::string& name) = 0;
		virtual RefPtr<IGPUImage> CreateImage(const IImage::CreationParams& params, MemoryPropertyFlags memoryFlags, const std::string& name) = 0;
		virtual RefPtr<IFramebuffer> CreateFramebuffer(IFramebuffer::Properties&& params) = 0;
		virtual RefPtr<IGPURenderPass> CreateRenderPass(const RenderPassParams& params) = 0;
		virtual RefPtr<IDescriptorPool> CreateDescriptorPool(const DescriptorPoolParams& params) = 0;
		virtual RefPtr<IDescriptorSetLayout> CreateDescriptorSetLayout(const DescriptorSetLayoutParams& params) = 0;
		virtual RefPtr<IBinarySemaphore> CreateBinarySemaphore() const = 0;
		virtual SamplerIndex CreateSampler(const SamplerCreateInfo& params) = 0;
		virtual RefPtr<IQueue> GetGraphicsQueue() const = 0;
		virtual ImageFormat GetSwapchainFormat() = 0;

		virtual void BeginFrame() = 0;
		virtual void Present() = 0;
		virtual ~IDevice() {}

		//Derived from IServiceProviderHolder
		IServiceProvider* GetServiceProvider() const override{ return m_services; }
	protected:
		IServiceProvider* m_services;
	};
}