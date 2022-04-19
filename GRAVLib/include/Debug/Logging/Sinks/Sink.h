#pragma once

#include "GRAVLibCore.h"
#include "Concurrency/Locks/Locks.h"
#include "ISink.h"

namespace GRAVLib::Debug::Logging::Sinks
{
	// Base sink class which handles locking. Implementations do not need to worry about locking
	class GRAVLibAPI sink : public ISink
	{
	public:
		sink();
		sink(const sink& other) = delete;
		sink(sink&& other) noexcept;

		sink& operator= (const sink& other) = delete;
		sink& operator= (sink&& other) noexcept;
		virtual ~sink();

		// Log a message
		virtual void log(const logMessage& message) final;
		// Flush the sink
		virtual void flush() final;

		virtual logVerbosity verbosity() const override;
		virtual void setVerobisty(logVerbosity verbosity) override;
		virtual bool shouldLog(logVerbosity verbosity) const override;
	protected:
		// Sink a message in the sink
		virtual void sinkMessage(const logMessage& message) = 0;
		// Virtual flush
		virtual void flush_() = 0;

	protected:
		GRAVLib_AUTO_MUTEX;

		verbosity_t m_Verbosity;
	};

	inline logVerbosity sink::verbosity() const
	{
		return m_Verbosity.load(std::memory_order_acquire);
	}
	inline void GRAVLib::Debug::Logging::Sinks::sink::setVerobisty(logVerbosity verbosity)
	{
		m_Verbosity.store(verbosity, std::memory_order_release);
	}
	inline bool sink::shouldLog(logVerbosity verbosity) const
	{
		return this->verbosity() >= verbosity;
	}
}