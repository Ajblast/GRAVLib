#include "GRAVLib/Debug/Logging/Sinks/MSVCSink.h"

#if GRAVLib_PLATFORM == GRAVLib_PLATFORM_WINDOWS
#ifndef	WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define NOMINMAX
#include <Windows.h> // For OutputDebugString()


void GRAVLib::Debug::Logging::Sinks::msvcSink::sinkMessage(const logMessage& message)
{
	OutputDebugStringA(message.m_Payload.data());
}

void GRAVLib::Debug::Logging::Sinks::msvcSink::flush_()
{
}

#endif