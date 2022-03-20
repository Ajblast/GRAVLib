#pragma once

#include "GRAVLibCore.h"
#include <atomic>
#include <format>

#define GRAVLib_VERBOSITY_TRACE		0
#define GRAVLib_VERBOSITY_DEBUG		1
#define GRAVLib_VERBOSITY_INFO		2
#define GRAVLib_VERBOSITY_WARN		3
#define GRAVLib_VERBOSITY_ERROR		4
#define GRAVLib_VERBOSITY_CRITICAL	5
#define GRAVLib_VERBOSITY_OFF		6

#define GRAVLib_VERBOSITY_STRING_OFF		"OFF"
#define GRAVLib_VERBOSITY_STRING_CRITICAL	"CRITICAL"
#define GRAVLib_VERBOSITY_STRING_ERROR		"ERROR"
#define GRAVLib_VERBOSITY_STRING_WARN		"WARN"
#define GRAVLib_VERBOSITY_STRING_INFO		"INFO"
#define GRAVLib_VERBOSITY_STRING_DEBUG		"DEBUG"
#define GRAVLib_VERBOSITY_STRING_TRACE		"TRACE"

namespace GRAVLib::Debug::Logging
{
	enum class logVerbosity : uint8
	{
		trace		= GRAVLib_VERBOSITY_TRACE,
		debug		= GRAVLib_VERBOSITY_DEBUG,
		info		= GRAVLib_VERBOSITY_INFO,
		warn		= GRAVLib_VERBOSITY_WARN,
		error		= GRAVLib_VERBOSITY_ERROR,
		critical	= GRAVLib_VERBOSITY_CRITICAL,
		off			= GRAVLib_VERBOSITY_OFF
	};

	typedef std::atomic<logVerbosity> verbosity_t;
}

template<>
struct std::formatter<GRAVLib::Debug::Logging::logVerbosity> : std::formatter<std::string_view>
{
	template<typename FormatContext>
	auto format(const GRAVLib::Debug::Logging::logVerbosity& verbosity, FormatContext& ctx)
	{
		std::string verbosityName;
		switch (priority)
		{
		case GRAVLib::Debug::Logging::logVerbosity::off:
			verbosityName = GRAVLib_VERBOSITY_STRING_OFF;
			break;
		case GRAVLib::Debug::Logging::logVerbosity::trace:
			verbosityName = GRAVLib_VERBOSITY_STRING_TRACE;
			break;
		case GRAVLib::Debug::Logging::logVerbosity::debug:
			verbosityName = GRAVLib_VERBOSITY_STRING_DEBUG;
			break;
		case GRAVLib::Debug::Logging::logVerbosity::info:
			verbosityName = GRAVLib_VERBOSITY_STRING_INFO;
			break;
		case GRAVLib::Debug::Logging::logVerbosity::warn:
			verbosityName = GRAVLib_VERBOSITY_STRING_WARN;
			break;
		case GRAVLib::Debug::Logging::logVerbosity::error:
			verbosityName = GRAVLib_VERBOSITY_STRING_ERROR;
			break;
		case GRAVLib::Debug::Logging::logVerbosity::critical:
			verbosityName = GRAVLib_VERBOSITY_STRING_CRITICAL;
			break;
		default:
			priorityName = "UNKNOWN";
			break;
		}

		return format_to(ctx.out(), "[{}]", verbosityName);
	}
};
