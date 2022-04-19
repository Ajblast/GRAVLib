#pragma once

#include "GRAVLib/GRAVLibCore.h"
#include "GRAVLib/Debug/Logging/Loggers/LogVerbosity.h"
#include <string>
#include <string_view>

namespace GRAVLib::Debug::Logging
{
	// A message that can be sent through a logger
	struct GRAVLibAPI logMessage
	{
	public:
		logMessage();
		logMessage(std::string_view loggerName, logVerbosity verbosity, std::string_view payload);
		logMessage(const logMessage& other);
		logMessage(logMessage&& other) noexcept;

		logMessage& operator=(const logMessage& other);
		logMessage& operator=(logMessage&& other) noexcept;
		~logMessage();

	public:
		std::string_view m_LoggerName;	// View into originating logger name
		logVerbosity m_Verbosity;		// Message verbosity
		std::string_view m_Payload;		// View into original string
	};
}