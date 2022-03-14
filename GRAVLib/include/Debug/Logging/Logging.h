#pragma once

#include "Loggers/LogManager.h"
#include <string>

namespace GRAVLib::Debug::Logging
{
	GRAVLibAPI void reportAssertionFailure(const std::string& expression, const std::string& file, size_t line);
}

#define GRAVLib_LOG_CRITICAL(format, ...)	GRAVLib::Debug::Logging::logManager::getInstance().defaultLogger()->log(GRAVLib::Debug::Logging::logVerbosity::critical,	format, __VA_ARGS__)
#define GRAVLib_LOG_ERROR(format, ...)		GRAVLib::Debug::Logging::logManager::getInstance().defaultLogger()->log(GRAVLib::Debug::Logging::logVerbosity::error,		format, __VA_ARGS__)
#define GRAVLib_LOG_WARN(format, ...)		GRAVLib::Debug::Logging::logManager::getInstance().defaultLogger()->log(GRAVLib::Debug::Logging::logVerbosity::warn,		format, __VA_ARGS__)
#define GRAVLib_LOG_INFO(format, ...)		GRAVLib::Debug::Logging::logManager::getInstance().defaultLogger()->log(GRAVLib::Debug::Logging::logVerbosity::info,		format, __VA_ARGS__)
#define GRAVLib_LOG_DEBUG(format, ...)		GRAVLib::Debug::Logging::logManager::getInstance().defaultLogger()->log(GRAVLib::Debug::Logging::logVerbosity::debug,		format, __VA_ARGS__)
#define GRAVLib_LOG_TRACE(format, ...)		GRAVLib::Debug::Logging::logManager::getInstance().defaultLogger()->log(GRAVLib::Debug::Logging::logVerbosity::trace,		format, __VA_ARGS__)

#define GRAVLib_LOG_LINE_CRITICAL(format, ...)	GRAVLib::Debug::Logging::logManager::getInstance().defaultLogger()->logLine(GRAVLib::Debug::Logging::logVerbosity::critical,	format, __VA_ARGS__)
#define GRAVLib_LOG_LINE_ERROR(format, ...)		GRAVLib::Debug::Logging::logManager::getInstance().defaultLogger()->logLine(GRAVLib::Debug::Logging::logVerbosity::error,		format, __VA_ARGS__)
#define GRAVLib_LOG_LINE_WARN(format, ...)		GRAVLib::Debug::Logging::logManager::getInstance().defaultLogger()->logLine(GRAVLib::Debug::Logging::logVerbosity::warn,		format, __VA_ARGS__)
#define GRAVLib_LOG_LINE_INFO(format, ...)		GRAVLib::Debug::Logging::logManager::getInstance().defaultLogger()->logLine(GRAVLib::Debug::Logging::logVerbosity::info,		format, __VA_ARGS__)
#define GRAVLib_LOG_LINE_DEBUG(format, ...)		GRAVLib::Debug::Logging::logManager::getInstance().defaultLogger()->logLine(GRAVLib::Debug::Logging::logVerbosity::debug,		format, __VA_ARGS__)
#define GRAVLib_LOG_LINE_TRACE(format, ...)		GRAVLib::Debug::Logging::logManager::getInstance().defaultLogger()->logLine(GRAVLib::Debug::Logging::logVerbosity::trace,		format, __VA_ARGS__)