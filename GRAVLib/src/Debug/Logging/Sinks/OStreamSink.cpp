#include "GRAVLib/Debug/Logging/Sinks/OStreamSink.h"

GRAVLib::Debug::Logging::Sinks::ostreamSink::ostreamSink(std::ostream& os, bool forceFlush) : m_OStream(os), m_ForceFlush(forceFlush)
{

}

void GRAVLib::Debug::Logging::Sinks::ostreamSink::sinkMessage(const logMessage& message)
{
	m_OStream.write(message.m_Payload.data(), message.m_Payload.size());
	if (m_ForceFlush)
		m_OStream.flush();
}

void GRAVLib::Debug::Logging::Sinks::ostreamSink::flush_()
{
	m_OStream.flush();
}
