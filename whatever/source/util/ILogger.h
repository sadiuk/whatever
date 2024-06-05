#pragma once
#include "util/RefPtr.h"
#include "IService.h"
#include <string>
#include <fmt/format.h>
#include <fmt/color.h>
namespace wtv
{

	struct ILogger : public IService
	{
		IMPL_ISERVICE(ILogger);

		enum class Severity
		{
			Debug,
			Info,
			Warn,
			Error
		};
		void Log(Severity severity, const std::string_view& message, auto&&... args)
		{
			std::string outMsg = fmt::format(fmt::runtime(message.data()), std::forward<decltype(args)>(args)...);
			LogImpl(severity, outMsg);
		}
	protected:
		auto GetConsoleColor(Severity severity)
		{
			using enum Severity;
			switch (severity)
			{
			case Debug:
				return fmt::fg(fmt::color::slate_gray);
			case Info:
				return fmt::fg(fmt::color::silver);
			case Warn:
				return fmt::fg(fmt::color::yellow);
			case Error:
				return fmt::fg(fmt::color::red);
			}
		}
		virtual void LogImpl(Severity severity, const std::string& msg) = 0;
	};

}