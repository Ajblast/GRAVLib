#include "GRAVLib/Debug/Logging/Loggers/LogManager.h"
#include "GRAVLib/Debug/Logging/Sinks/FileSink.h"

GRAVLib_SINGLETON_SET_INSTANCE(GRAVLib::Debug::Logging::logManager);

GRAVLib::Debug::Logging::logManager::logManager() : 
	m_Verbosity(logVerbosity::off), m_FlushVerbosity(logVerbosity::off)
{
}
GRAVLib::Debug::Logging::logManager::logManager(logVerbosity maximalVerbosity, logVerbosity minimalFlushVerbosity) :
	m_Verbosity(maximalVerbosity), m_FlushVerbosity(minimalFlushVerbosity)
{
}
GRAVLib::Debug::Logging::logManager::~logManager()
{
	GRAVLib_LOCK_AUTO_MUTEX;

	defaultLogger()->log(logVerbosity::debug, "{}: Shutting down log manager.", GRAVLib_CLEAN_FUNC_SIG);

	// Remove all of the loggers
	removeAll();
}

void GRAVLib::Debug::Logging::logManager::setDefaultLogger(ref<logger> logger)
{
	GRAVLib_LOCK_AUTO_MUTEX;

	GRAV_ASSERT_NE(logger, nullptr); // Attempting to set default logger to nullptr

	// Remove previous default logger
	m_DefaultLogger.reset();

	// Register the logger if it is new
	if (get(logger->name()) == nullptr)
		registerLogger(logger);


	// Set the new default logger
	m_DefaultLogger = logger;
}


void GRAVLib::Debug::Logging::logManager::setVerbosity(logVerbosity verbosity)
{
	GRAVLib_LOCK_AUTO_MUTEX;

	for (auto& logger : m_Loggers)
		logger.second->setVerbosity(verbosity);

	m_Verbosity.store(verbosity, std::memory_order_release);
}
void GRAVLib::Debug::Logging::logManager::setFlushVerbosity(logVerbosity verbosity)
{
	GRAVLib_LOCK_AUTO_MUTEX;

	for (auto& logger : m_Loggers)
		logger.second->setFlushVerbosity(verbosity);

	m_FlushVerbosity.store(verbosity, std::memory_order_release);
}
void GRAVLib::Debug::Logging::logManager::flushAll()
{
	GRAVLib_LOCK_AUTO_MUTEX;

	for (auto& logger : m_Loggers)
		logger.second->flush();
}


GRAVLib::ref<GRAVLib::Debug::Logging::logger> GRAVLib::Debug::Logging::logManager::get(const std::string& loggerName)
{
	GRAVLib_LOCK_AUTO_MUTEX;

	auto found = m_Loggers.find(loggerName);
	return found == m_Loggers.end() ? nullptr : found->second;
}
void GRAVLib::Debug::Logging::logManager::registerLogger(ref<logger> logger)
{
	GRAVLib_LOCK_AUTO_MUTEX;

	const std::string& loggerName = logger->name();

	// Assert that logger of the same name is not being registered
	if (get(loggerName) == nullptr)
		return;


	// Set the maximal verbosity allowed
	logVerbosity verbosity = m_Verbosity.load(std::memory_order_acquire);
	if (logger->verbosity() > verbosity)
		logger->setVerbosity(verbosity);

	logVerbosity flushVerbosity = m_FlushVerbosity.load(std::memory_order_acquire);
	// Set the minimal flush verbosity
	if (logger->flushVerbosity() < flushVerbosity)
		logger->setFlushVerbosity(flushVerbosity);

	// The first registered logger is this logger
	if (m_DefaultLogger == nullptr)
		m_DefaultLogger = logger;


	m_Loggers[loggerName] = logger;
}
void GRAVLib::Debug::Logging::logManager::removeLogger(const std::string& loggerName)
{
	GRAVLib_LOCK_AUTO_MUTEX;

	m_Loggers.erase(loggerName);

	if (m_DefaultLogger && m_DefaultLogger->name() == loggerName)
		m_DefaultLogger.reset();
}
void GRAVLib::Debug::Logging::logManager::removeAll()
{
	GRAVLib_LOCK_AUTO_MUTEX;

	m_Loggers.clear();
	m_DefaultLogger.reset();
}