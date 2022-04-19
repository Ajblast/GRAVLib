#pragma once

#include "GRAVLib/GRAVLibCore.h"
#include "GRAVLib/Debug/Logging/Sinks/Sink.h"

#if GRAVLib_PLATFORM == GRAVLib_PLATFORM_WINDOWS
namespace GRAVLib::Debug::Logging::Sinks
{
	class GRAVLibAPI msvcSink final : public sink
	{
	public:
		msvcSink() = default;
		virtual ~msvcSink() = default;
	protected:
		// Inherited via baseSink
		virtual void sinkMessage(const logMessage& message) final;
		virtual void flush_() final;
	};
}
#else
#error "MSVCSink is only supported on windows platforms!"
#endif