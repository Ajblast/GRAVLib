#pragma once

#include "GRAVLib/GRAVLibCore.h"
#include "GRAVLib/Concurrency/Locks/Locks.h"
#include "GRAVLib/Debug/Logging/Loggers/Logger.h"
#include "GRAVLib/Debug/Logging/Sinks/ISink.h"

#include <string>
#include <memory>
#include <unordered_map>
#include <atomic>

#define GRAVLib_DEFAULT_LOGGER_NAME ""

namespace GRAVLib::Debug::Logging
{
	// TODO: Allow asynchronous logging
	// TODO: Allow flushing of the sinks in interval time

	// Manager to contol all registered loggers
	class GRAVLibAPI logManager : public singleton<logManager>
	{
	public:
		logManager();
		logManager(logVerbosity maximalVerbosity, logVerbosity minimalFlushVerbosity);
		virtual ~logManager();

		#pragma region DefaultLogger
		// Get the default logger
		ref<logger> defaultLogger();
		// Set the default logger
		void setDefaultLogger(ref<logger> logger);
		#pragma endregion

		#pragma region Verbosity
		// Set the verbosity of all loggers
		void setVerbosity(logVerbosity verbosity);
		// Set the flush verbosity of all loggers
		void setFlushVerbosity(logVerbosity verbosity);
		#pragma endregion

		// Flush all of the loggers
		void flushAll();

		#pragma region Loggers		
		// Get a logger based on its name
		ref<logger> get(const std::string& loggerName);

		// Register a logger
		void registerLogger(ref<logger> logger);
		// Remove a logger by its name
		void removeLogger(const std::string& loggerName);
		// Remove all of the loggers
		void removeAll();
		#pragma endregion


		GRAVLib_SINGLETON_INSTANCE(logManager);
	private:
		GRAVLib_AUTO_MUTEX;

		verbosity_t m_FlushVerbosity;
		verbosity_t m_Verbosity;

		ref<logger> m_DefaultLogger;
		std::unordered_map<std::string, ref<logger>> m_Loggers;
		std::unordered_map<std::string, logVerbosity> m_LoggerVerbosities;
	};

	inline ref<logger> logManager::defaultLogger()
	{
		GRAVLib_LOCK_AUTO_MUTEX;

		return m_DefaultLogger;
	}
}