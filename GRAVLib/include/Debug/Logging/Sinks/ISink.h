#pragma once

#include "GRAVLibCore.h"
#include "Debug/Logging/Loggers/LogMessage.h"
#include "Debug/Logging/Loggers/LogVerbosity.h"

namespace GRAVLib::Debug::Logging::Sinks
{
	// A sink is a data object that receives logging inputs
	class GRAVLibAPI ISink
	{
	public:
		virtual ~ISink() = default;
		virtual void log(const logMessage& message) = 0;
		virtual void flush() = 0;

		virtual void setVerobisty(logVerbosity verbosity) = 0;
		virtual bool shouldLog(logVerbosity verbosity) const = 0;
		virtual logVerbosity verbosity() const = 0;
	};
}
