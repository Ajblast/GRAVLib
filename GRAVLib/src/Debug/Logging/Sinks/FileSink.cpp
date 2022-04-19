#include "GRAVLib/Debug/Logging/Sinks/FileSink.h"

GRAVLib::Debug::Logging::Sinks::fileSink::fileSink(const std::string& filePath, bool truncate) : 
	m_File(filePath, GRAVLib::IO::fileMode::OUTPUT | GRAVLib::IO::fileMode::APPEND, false)
{
}

void GRAVLib::Debug::Logging::Sinks::fileSink::sinkMessage(const logMessage& message)
{
	// Write the message to the file
	m_File.write(message.m_Payload);
}

void GRAVLib::Debug::Logging::Sinks::fileSink::flush_()
{
	// Flush the file
	m_File.flush();
}
