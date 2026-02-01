#pragma once
#include "ICPUImageLoader.h"
namespace wtv
{
	class STBCPUImageLoader : public ICPUImageLoader
	{
		ICPUImage* Load(const std::filesystem::path& path) override;
	};
}