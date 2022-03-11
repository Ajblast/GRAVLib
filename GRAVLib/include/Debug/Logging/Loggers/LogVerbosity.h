#pragma once

#include "GRAVLibCore.h"
#include <atomic>
#include <string>

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


	static const std::string verbosityNames[]
	{
		GRAVLib_VERBOSITY_STRING_OFF,
		GRAVLib_VERBOSITY_STRING_CRITICAL,
		GRAVLib_VERBOSITY_STRING_ERROR,
		GRAVLib_VERBOSITY_STRING_WARN,
		GRAVLib_VERBOSITY_STRING_INFO,
		GRAVLib_VERBOSITY_STRING_DEBUG,
		GRAVLib_VERBOSITY_STRING_TRACE
	};

	GRAVLibAPI inline std::string toString(GRAVLib::Debug::Logging::logVerbosity verbosity)
	{
		return verbosityNames[static_cast<int>(verbosity)];
	}

	GRAVLibAPI inline GRAVLib::Debug::Logging::logVerbosity logVerbosityToEnum(const std::string& name)
	{
		int level = 0;

		for (auto& levelstr : verbosityNames)
		{
			if (levelstr.compare(name))
				return static_cast<GRAVLib::Debug::Logging::logVerbosity>(level);

			level++;
		}

		return GRAVLib::Debug::Logging::logVerbosity::off;
	}
	GRAVLibAPI inline GRAVLib::Debug::Logging::logVerbosity logVerbosityToEnum(const char* name)
	{
		return logVerbosityToEnum(std::string(name));
	}
}