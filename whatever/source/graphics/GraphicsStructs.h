#pragma once
#include "GraphicsConstants.h"

#include <vector>
#include <string>
#include <filesystem>

namespace wtv
{
	struct Rect2D
	{
		int32_t x = 0;
		int32_t y = 0;
		uint32_t width = 0;
		uint32_t height = 0;
	};
	struct StageDesc
	{
		ShaderStage stage;
		std::string entryPoint;
		std::string path;
	};
	struct ViewportInfo
	{
		uint32_t x = 0, y = 0;
		uint32_t width, height;
	};
	struct AttachmentBlendState
	{
		bool blendEnable = true;
		BlendFactor srcColorBlendFactor = BlendFactor::One;
		BlendFactor dstColorBlendFactor = BlendFactor::One;
		BlendOperation colorBlendOperation = BlendOperation::Add;
		BlendFactor srcAlphaBlendFactor = BlendFactor::One;
		BlendFactor dstAlphaBlendFactor = BlendFactor::One;
		BlendOperation alphaBlendOperation = BlendOperation::Add;
	};
	struct BlendStateInfo
	{
		std::vector<AttachmentBlendState> attachmentBlendStates;
	};
	struct StencilOperation
	{
		StencilTestOperation failOperation = StencilTestOperation::Zero;
		StencilTestOperation passOperation = StencilTestOperation::Zero;
		StencilTestOperation depthFailOperation = StencilTestOperation::Zero;
		CompareOperation compareOperation = CompareOperation::Never;
		uint32_t compareMask = 0;
		uint32_t writeMask = 0;
		uint32_t reference = 0;
	};
	struct DepthStencilInfo
	{
		bool depthTestEnable = true;
		bool depthWriteEnable = true;
		CompareOperation depthTestPassResult;
		bool stencilTestEnabled = false;
		StencilOperation backStencilOp;
		StencilOperation frontStencilOp;
	};
	struct RasterInfo
	{
		bool enableDepthClamp = false;
		FrontFace frontFace;
		CullMode cullMode;
		PolygonMode polygonMode;
	};
	struct DescriptorSetLayoutEntry
	{
		uint32_t binding;
		DescriptorType type;
		uint32_t descriptorCount = 1;
		ShaderStageFlags stageFlags;
	};
	using DescriptorSetLayout = std::vector<DescriptorSetLayoutEntry>;
}