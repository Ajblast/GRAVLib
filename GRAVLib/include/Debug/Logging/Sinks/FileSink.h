#pragma once

#include "GRAVLibCore.h"
#include "Sink.h"
#include "IO/Files/TextFile.h"

namespace GRAVLib::Debug::Logging::Sinks
{
	class GRAVLibAPI fileSink final : public sink
	{
	public:
		explicit fileSink(const std::string& filePath, bool truncate = false);

		std::string filePath() const;

	private:
		virtual void sinkMessage(const logMessage& message) override;
		virtual void flush_() override;

	private:
		GRAVLib::IO::textFile m_File;
	};

	inline std::string fileSink::filePath() const
	{
		return m_File.filePath();
	}

}