#pragma once

#include "GRAVLibCore.h"
#include "Sink.h"
#include <ostream>

namespace GRAVLib::Debug::Logging::Sinks
{
	// Sink for an ostream
	class GRAVLibAPI ostreamSink : public sink
	{
	public:
		// Needs to be explicit because it would be considered a converting constructor
		explicit ostreamSink(std::ostream& os, bool forceFlush = false);

	protected:
		virtual void sinkMessage(const logMessage& message) override;
		virtual void flush_() override;

	protected:
		std::ostream& m_OStream;
		bool m_ForceFlush;
	};
}