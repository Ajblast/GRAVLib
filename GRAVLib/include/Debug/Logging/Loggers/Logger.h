#pragma once

#include "GRAVLibCore.h"
#include "LogVerbosity.h"
#include "LogMessage.h"
#include "Debug/Logging/Sinks/ISink.h"

#include <initializer_list> // For bracketed list of objects

#include <atomic>
#include <string>
#include <vector>

// TODO: Allow a logger (More like Sinks) to have the capability of having a backlog of message such that message don't have to be logged immediately
// TODO: For log line, determine if should be using \n or \r\n

namespace GRAVLib::Debug::Logging
{
	// Each logger has a name, verbosity, and sinks
	// On Write
	//	1. Check if verbosity is enough
	//	2. Call sinks to handle logging
	//	3. Each sink has its own sink formatting

	// Logger to log information to a file
	class GRAVLibAPI logger
	{
	public:
		logger();
		template<typename it>
		logger(const std::string& name, logVerbosity verbosity, logVerbosity flushVerbosity, it begin, it end);
		logger(const std::string& name, logVerbosity verbosity, logVerbosity flushVerbosity, std::initializer_list<ref<Sinks::ISink>> sinks);
		logger(const std::string& name, logVerbosity verbosity, logVerbosity flushVerbosity, ref<Sinks::ISink> sink);

		logger(const logger& other) = delete;
		logger(logger&& other) noexcept;
		logger& operator= (const logger& other) = delete;
		logger& operator= (logger&& other) noexcept;

		~logger();

		#pragma region Logging
		template<typename... Args>
		void log(logVerbosity verbosity, const std::string& format, Args&&... args);
		template<typename... Args>
		void logLine(logVerbosity verbosity, const std::string& format, Args&&... args);
		#pragma endregion

		#pragma region Verbosity
		logVerbosity verbosity() const;
		void setVerbosity(logVerbosity verbosity);
		bool shouldLog(logVerbosity verbosity) const;

		logVerbosity flushVerbosity() const;
		void setFlushVerbosity(logVerbosity verbosity);
		#pragma endregion

		#pragma region Flusing
		void flush();
		#pragma endregion

		const std::string& name() const;

	protected:
		bool shouldFlush(const logMessage& message);

	private:
		void logMessage_(logMessage& message, bool logEnabled);
		void sinkMessage(logMessage& message);

	private:
		std::string m_Name;
		std::vector<ref<Sinks::ISink>> m_Sinks;

		verbosity_t m_Verbosity;
		verbosity_t m_FlushVerbosity;
	};

	template<typename it>
	GRAVLib::Debug::Logging::logger::logger(const std::string& name, logVerbosity verbosity, logVerbosity flushVerbosity, it begin, it end) :
		m_Name(name), m_Verbosity(verbosity), m_FlushVerbosity(flushVerbosity), m_Sinks(begin, end)
	{}
	template<typename... Args>
	void GRAVLib::Debug::Logging::logger::log(logVerbosity verbosity, const std::string& format, Args&&... args)
	{
		bool logEnabled = shouldLog(verbosity);

		if (logEnabled == false)
			return;

		try
		{
			// Format the string
			std::string formattedString = std::vformat(format, std::make_format_args(std::forward<Args>(args)...));

			// Create the message
			logMessage msg = logMessage(name(), this->verbosity(), formattedString);

			logMessage_(msg, logEnabled);
		}
		catch (const std::format_error&)
		{
			// TODO: Create custom format exception
			throw;
		}
		catch (const std::bad_alloc&)
		{
			// TODO: Create custom bad allocation exception
			throw;
		}
	}
	template<typename... Args>
	void GRAVLib::Debug::Logging::logger::logLine(logVerbosity verbosity, const std::string& format, Args&&... args)
	{
		bool logEnabled = shouldLog(verbosity);

		if (logEnabled == false)
			return;

		try
		{
			// Format the string
			std::string formattedString = std::vformat(format, std::make_format_args(std::forward<Args>(args)...)) + "\n";

			// Create the message
			logMessage msg = logMessage(name(), this->verbosity(), formattedString);

			logMessage_(msg, logEnabled);
		}
		catch (std::format_error)
		{
			// TODO: Create custom format exception
			throw;
		}
		catch (std::bad_alloc)
		{
			// TODO: Create custom bad allocation exception
			throw;
		}
		catch (std::exception)
		{
			throw;
		}
	}


	inline const std::string& logger::name() const 
	{
		return m_Name;
	}

	inline logVerbosity logger::verbosity() const
	{
		return m_Verbosity.load(std::memory_order_acquire);
	}
	inline bool logger::shouldLog(logVerbosity verbosity) const 
	{
		return verbosity >= this->verbosity();
	}
	inline void logger::setVerbosity(logVerbosity verbosity)
	{
		m_Verbosity.store(verbosity, std::memory_order_release);
	}

	inline logVerbosity logger::flushVerbosity() const
	{
		return m_FlushVerbosity.load(std::memory_order_acquire);
	}
	inline void logger::setFlushVerbosity(logVerbosity verbosity)
	{
		m_FlushVerbosity.store(verbosity, std::memory_order_release);
	}
	inline bool logger::shouldFlush(const logMessage& message)
	{
		return message.m_Verbosity >= flushVerbosity() && message.m_Verbosity != logVerbosity::off;
	}
}
