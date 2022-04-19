#include "GRAVLib/Debug/Logging/Loggers/Logger.h"
#include "GRAVLib/Concurrency/Atomics/Atomics.h"

GRAVLib::Debug::Logging::logger::logger()
{}
GRAVLib::Debug::Logging::logger::logger(const std::string& name, logVerbosity verbosity, logVerbosity flushVerbosity, std::initializer_list<ref<Sinks::ISink>> sinks) :
	logger(name, verbosity, flushVerbosity, sinks.begin(), sinks.end())
{}
GRAVLib::Debug::Logging::logger::logger(const std::string& name, logVerbosity verbosity, logVerbosity flushVerbosity, ref<Logging::Sinks::ISink> sink) :
	logger(name, verbosity, flushVerbosity, { sink })
{}
GRAVLib::Debug::Logging::logger::logger(logger&& other) noexcept
{
	*this = std::move(other);
}
GRAVLib::Debug::Logging::logger& GRAVLib::Debug::Logging::logger::operator=(logger&& other) noexcept
{
	if (this != &other)
	{
		// Steal Resources
		m_Name = std::move(other.m_Name);
		m_Sinks = std::move(other.m_Sinks);

		GRAVLib_ATOMIC_SWAP(m_Verbosity, other.m_Verbosity);
		GRAVLib_ATOMIC_SWAP(m_FlushVerbosity, other.m_FlushVerbosity);
	}

	return *this;
}
GRAVLib::Debug::Logging::logger::~logger()
{
}


void GRAVLib::Debug::Logging::logger::flush()
{
	for (auto& sink : m_Sinks)
	{
		sink->flush();
	}
}

void GRAVLib::Debug::Logging::logger::logMessage_(Logging::logMessage& message, bool logEnabled)
{
	if (logEnabled)
		sinkMessage(message);
}

void GRAVLib::Debug::Logging::logger::sinkMessage(Logging::logMessage& message)
{
	for (auto& sink : m_Sinks)
	{
		if (sink->shouldLog(message.m_Verbosity))
		{
			sink->log(message);
		}
	}

	if (shouldFlush(message))
	{
		flush();
	}
}