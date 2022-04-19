#include "GRAVLib/Debug/Logging/Logging.h"

// TODO: Allow time stamping and allowing categories
void GRAVLib::Debug::Logging::reportAssertionFailure(const std::string& expression, const std::string& file, size_t line)
{
	// Get the logger
	//auto logger = GRAVLib::Debug::Logging::logManager::getInstance()->defaultLogger();

	// Concatenate the strings
	//std::stringstream ss;
	//ss << "Expression: " << expression << " | File: " << file << " | Line: " << line;

	// Log the assertion
	GRAVLib::Debug::Logging::logManager::getInstance().defaultLogger()->logLine(
		logVerbosity::critical,
		"Expression: {} | File: {} | Line: {}", expression, file, line
	);
}