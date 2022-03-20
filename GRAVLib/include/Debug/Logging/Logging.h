#pragma once

#include "Loggers/LogManager.h"
#include <string>

namespace GRAVLib::Debug::Logging
{
	GRAVLibAPI void reportAssertionFailure(const std::string& expression, const std::string& file, size_t line);
}

#define GRAVLib_LOG_CRITICAL(format, ...)	if(GRAVLib::Debug::Logging::logManager::getInstancePtr()) GRAVLib::Debug::Logging::logManager::getInstance().defaultLogger()->log(GRAVLib::Debug::Logging::logVerbosity::critical,	format, __VA_ARGS__)
#define GRAVLib_LOG_ERROR(format, ...)		if(GRAVLib::Debug::Logging::logManager::getInstancePtr()) GRAVLib::Debug::Logging::logManager::getInstance().defaultLogger()->log(GRAVLib::Debug::Logging::logVerbosity::error,		format, __VA_ARGS__)
#define GRAVLib_LOG_WARN(format, ...)		if(GRAVLib::Debug::Logging::logManager::getInstancePtr()) GRAVLib::Debug::Logging::logManager::getInstance().defaultLogger()->log(GRAVLib::Debug::Logging::logVerbosity::warn,		format, __VA_ARGS__)
#define GRAVLib_LOG_INFO(format, ...)		if(GRAVLib::Debug::Logging::logManager::getInstancePtr()) GRAVLib::Debug::Logging::logManager::getInstance().defaultLogger()->log(GRAVLib::Debug::Logging::logVerbosity::info,		format, __VA_ARGS__)
#define GRAVLib_LOG_DEBUG(format, ...)		if(GRAVLib::Debug::Logging::logManager::getInstancePtr()) GRAVLib::Debug::Logging::logManager::getInstance().defaultLogger()->log(GRAVLib::Debug::Logging::logVerbosity::debug,		format, __VA_ARGS__)
#define GRAVLib_LOG_TRACE(format, ...)		if(GRAVLib::Debug::Logging::logManager::getInstancePtr()) GRAVLib::Debug::Logging::logManager::getInstance().defaultLogger()->log(GRAVLib::Debug::Logging::logVerbosity::trace,		format, __VA_ARGS__)

#define GRAVLib_LOG_LINE_CRITICAL(format, ...)	if(GRAVLib::Debug::Logging::logManager::getInstancePtr()) GRAVLib::Debug::Logging::logManager::getInstance().defaultLogger()->logLine(GRAVLib::Debug::Logging::logVerbosity::critical,	format, __VA_ARGS__)
#define GRAVLib_LOG_LINE_ERROR(format, ...)		if(GRAVLib::Debug::Logging::logManager::getInstancePtr()) GRAVLib::Debug::Logging::logManager::getInstance().defaultLogger()->logLine(GRAVLib::Debug::Logging::logVerbosity::error,		format, __VA_ARGS__)
#define GRAVLib_LOG_LINE_WARN(format, ...)		if(GRAVLib::Debug::Logging::logManager::getInstancePtr()) GRAVLib::Debug::Logging::logManager::getInstance().defaultLogger()->logLine(GRAVLib::Debug::Logging::logVerbosity::warn,		format, __VA_ARGS__)
#define GRAVLib_LOG_LINE_INFO(format, ...)		if(GRAVLib::Debug::Logging::logManager::getInstancePtr()) GRAVLib::Debug::Logging::logManager::getInstance().defaultLogger()->logLine(GRAVLib::Debug::Logging::logVerbosity::info,		format, __VA_ARGS__)
#define GRAVLib_LOG_LINE_DEBUG(format, ...)		if(GRAVLib::Debug::Logging::logManager::getInstancePtr()) GRAVLib::Debug::Logging::logManager::getInstance().defaultLogger()->logLine(GRAVLib::Debug::Logging::logVerbosity::debug,		format, __VA_ARGS__)
#define GRAVLib_LOG_LINE_TRACE(format, ...)		if(GRAVLib::Debug::Logging::logManager::getInstancePtr()) GRAVLib::Debug::Logging::logManager::getInstance().defaultLogger()->logLine(GRAVLib::Debug::Logging::logVerbosity::trace,		format, __VA_ARGS__)