#pragma once
#include "ICPUImage.h"
#include "filesystem"

namespace wtv
{
	class ICPUImageLoader
	{
		virtual ICPUImage* Load(const std::filesystem::path& path) = 0;
	};
}