#include "Debug/Logging/Loggers/LogMessage.h"

GRAVLib::Debug::Logging::logMessage::logMessage() :
	m_Verbosity(logVerbosity::off)
{}
GRAVLib::Debug::Logging::logMessage::logMessage(std::string_view loggerName, logVerbosity verbosity, std::string_view payload)
	: m_LoggerName(loggerName), m_Verbosity(verbosity), m_Payload(payload)
{}
GRAVLib::Debug::Logging::logMessage::logMessage(const logMessage& other)
	: m_LoggerName(other.m_LoggerName), m_Verbosity(other.m_Verbosity), m_Payload(other.m_Payload)
{}
GRAVLib::Debug::Logging::logMessage::logMessage(logMessage&& other) noexcept
{
	*this = std::move(other);
}
GRAVLib::Debug::Logging::logMessage& GRAVLib::Debug::Logging::logMessage::operator=(const logMessage & other)
{
	if (this != &other)
	{
		m_LoggerName = other.m_LoggerName;
		m_Verbosity = other.m_Verbosity;
		m_Payload = other.m_Payload;
	}

	return *this;
}
GRAVLib::Debug::Logging::logMessage& GRAVLib::Debug::Logging::logMessage::operator=(logMessage&& other) noexcept
{
	if (this != &other)
	{
		m_LoggerName = std::move(other.m_LoggerName);
		m_Verbosity = std::move(other.m_Verbosity);
		m_Payload = std::move(other.m_Payload);
	}

	return *this;
}
GRAVLib::Debug::Logging::logMessage::~logMessage()
{
}