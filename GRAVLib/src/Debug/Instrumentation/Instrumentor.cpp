#include "Debug/Instrumentation/Instrumentor.h"
#include "Concurrency/Threads/Thread.h"
#include "IO/Exceptions/IOException.h"
#include <format>

GRAVLib_SINGLETON_SET_INSTANCE(GRAVLib::Debug::instrumentor);

GRAVLib::Debug::instrumentor::instrumentor() : 
	singleton(), m_CurrentSession(nullptr) 
{}
GRAVLib::Debug::instrumentor::~instrumentor()
{
	GRAVLib_LOCK_AUTO_MUTEX;

	endSession();
}

void GRAVLib::Debug::instrumentor::startSession(const std::string& sessionName, const std::string& filepath)
{
	GRAVLib_LOCK_AUTO_MUTEX;

	if (m_CurrentSession)
	{
		// Attempting to start a new session while one is in progress.
		//	End the current session and start a new one

		GRAVLib_LOG_LINE_ERROR("instrumentor::startSession('{}') when session '{}' already open.", sessionName, m_CurrentSession->m_SessionName);

		// End the current session
		internalEndSession();
	}

	// Start a new session
	try
	{
		m_OutputFile = IO::textFile(filepath, IO::fileMode::OUTPUT | IO::fileMode::TRUNCATE, true);

		GRAVLib_LOG_LINE_INFO("Start instrumentation session '{}'", sessionName);

		instrumentationSession session = { sessionName };
		m_CurrentSession = createScope<instrumentationSession>(std::move(session));
		writeSessionHeader();
	}
	catch (IO::Exceptions::ioException& e)
	{
		GRAVLib_LOG_LINE_ERROR("Instrumentor could not open the results file '{}'", filepath);
	}

}
void GRAVLib::Debug::instrumentor::endSession()
{
	GRAVLib_LOCK_AUTO_MUTEX;

	// End the session
	internalEndSession();
}
void GRAVLib::Debug::instrumentor::writeProfile(const profileResult& result)
{
	GRAVLib_LOCK_AUTO_MUTEX;

	if (m_CurrentSession)
	{
		std::stringstream json;

		auto elapsedDurationCount = result.m_ElapsedDuration.count();
		auto startPoint = result.m_StartPoint.count();

		json << std::setprecision(3) << std::fixed;
		json << ",{";
		json << "\"cat\":\"function\",";
		json << "\"dur\":" << elapsedDurationCount << ',';
		json << "\"name\":\"" << result.m_ProfileName << "\",";
		json << "\"ph\":\"X\",";
		json << "\"pid\":0,";
		json << "\"tid\":" << result.m_ThreadID.m_ThreadID << ",";
		json << "\"ts\":" << startPoint;
		json << "}";

		// Output the file if the current session is a valid one
		m_OutputFile.write(json.str());
	}
}
void GRAVLib::Debug::instrumentor::internalEndSession()
{
	GRAVLib_LOCK_AUTO_MUTEX;

	if (m_CurrentSession)
	{
		GRAVLib_LOG_LINE_INFO("End instrumentation session '{}'", m_CurrentSession->m_SessionName);

		// Write the footer and close the file
		writeSessionFooter();
		m_OutputFile.close();

		// Delete the current session
		m_CurrentSession = nullptr;
	}
}

void GRAVLib::Debug::instrumentor::writeSessionHeader()
{
	GRAVLib_LOCK_AUTO_MUTEX;

	m_OutputFile.write("{\"otherData\": {},\"traceEvents\":[{}");
}
void GRAVLib::Debug::instrumentor::writeSessionFooter()
{
	GRAVLib_LOCK_AUTO_MUTEX;

	m_OutputFile.write("]}");
}

GRAVLib::Debug::instrumentorStopwatch::instrumentorStopwatch(const std::string& name) : m_Name(name)
{
	// Start the stopwatch
	stopwatch.start();
}
GRAVLib::Debug::instrumentorStopwatch::~instrumentorStopwatch()
{
	if (stopwatch.isRunning())
	{
		stop();

		// Output the 
	}
}
void GRAVLib::Debug::instrumentorStopwatch::stop()
{
	stopwatch.pause();

	if (instrumentor::getInstancePtr())
		instrumentor::getInstance().writeProfile({
			m_Name,
			Time::microseconds(stopwatch.startTick().time_since_epoch()),
			stopwatch.deltaTime().elapsedMicrosecondsDuration(),
			GRAVLib::Concurrency::Threads::thread::getCurrentThreadID()
			});
}
