#include "ConsoleLogger.h"
namespace wtv
{
	void ConsoleLogger::LogImpl(Severity severity, const std::string& msg)
	{
		fmt::print(stdout, GetConsoleColor(severity), "{}\n", msg.c_str());
	}
}