#pragma once

#include "ILogger.h"

namespace wtv
{
	class ConsoleLogger : public ILogger
	{
		void LogImpl(ILogger::Severity severity, const std::string& msg) override;
	};
}