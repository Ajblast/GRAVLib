#include "Debug/Logging/Sinks/Sink.h"
#include "Concurrency/Atomics/Atomics.h"

GRAVLib::Debug::Logging::Sinks::sink::sink() : m_Verbosity(logVerbosity::off)
{
}
GRAVLib::Debug::Logging::Sinks::sink::sink(sink&& other) noexcept
{
	*this = std::move(other);
}
GRAVLib::Debug::Logging::Sinks::sink& GRAVLib::Debug::Logging::Sinks::sink::operator=(sink&& other) noexcept
{
	if (this != &other)
	{
		GRAVLib_LOCK_LOCKS(GRAVLib_AUTO_MUTEX_NAME, other.GRAVLib_AUTO_MUTEX_NAME);

		GRAVLib_ATOMIC_SWAP(m_Verbosity, other.m_Verbosity);
	}

	return *this;
}
GRAVLib::Debug::Logging::Sinks::sink::~sink()
{
}

void GRAVLib::Debug::Logging::Sinks::sink::log(const Logging::logMessage& message)
{
	GRAVLib_LOCK_AUTO_MUTEX;

	// Sink the message
	sinkMessage(message);
}

void GRAVLib::Debug::Logging::Sinks::sink::flush()
{
	GRAVLib_LOCK_AUTO_MUTEX;

	// Flush the sink
	flush_();
}
