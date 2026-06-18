#pragma once
#include "ICPUImageLoader.h"
namespace wtv
{
	class STBCPUImageLoader
	{
		CPUImageData Load(const std::filesystem::path& path);
	};
}