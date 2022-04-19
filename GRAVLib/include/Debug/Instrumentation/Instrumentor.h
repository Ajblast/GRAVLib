#pragma once

#include "GRAVLibCore.h" 
#include "Memory/Singleton.h"
#include "Concurrency/Threads/ThreadID.h"
#include "Concurrency/Locks/Locks.h"

#include "Time/Durations.h"
#include "Time/Stopwatch.h"
#include "IO/Files/TextFile.h"

#include <string>

namespace GRAVLib::Debug
{
	struct GRAVLibAPI profileResult
	{
		std::string m_ProfileName;

		Time::microseconds m_StartPoint;
		Time::microseconds m_ElapsedDuration;
		Concurrency::Threads::threadID m_ThreadID;

		// TODO: Total memory used
		// TODO: Memory allocated
		// TODO: Memory deallocated
	};
	struct GRAVLibAPI instrumentationSession
	{
		// TODO: Track memory allocations/deallocations
		// TODO: Potentially stop/pause a profiling session when a job is taken off and waiting for other 
		std::string m_SessionName;
	};

	class GRAVLibAPI instrumentor : public singleton<instrumentor>
	{
	public:
		instrumentor();
		instrumentor(const instrumentor& other) = delete;
		instrumentor(instrumentor&& other) noexcept = delete;
		instrumentor& operator=(const instrumentor& other) = delete;
		instrumentor& operator=(instrumentor&& other) noexcept = delete;
		virtual ~instrumentor();

		void startSession(const std::string& sessionName, const std::string& filepath);
		void endSession();

		void writeProfile(const profileResult& result);

		GRAVLib_SINGLETON_INSTANCE(instrumentor);
	private:
		void internalEndSession();

		void writeSessionHeader();
		void writeSessionFooter();
	private:
		GRAVLib_AUTO_MUTEX;

		scope<instrumentationSession> m_CurrentSession;
		IO::textFile m_OutputFile;
	};

	class GRAVLibAPI instrumentorStopwatch
	{
	public:
		instrumentorStopwatch(const std::string& name);
		~instrumentorStopwatch();

		void stop();
	private:
		std::string m_Name;			// Name of instrumented line
		Time::stopwatch stopwatch;	// Timer
	};

}

#ifdef GRAVLib_PROFILE
#define GRAVLib_PROFILE_START_SESSION(name, filepath) GRAVLib::Debug::instrumentor::getInstance().startSession(name, filepath)
#define GRAVLib_PROFILE_END_SESSION() GRAVLib::Debug::instrumentor::getInstance().endSession()

#if GRAVLib_PROFILE_CONSOLE
#include "Debug/Logging/Logging.h"
#define GRAVLib_PROFILE_SCOPE_LINE2(name, line) GRAVLib::Debug::instrumentorStopwatch GRAVLib_ANONYMOUS_VARIABLE(stopwatch) (name);\
												GRAVLib_LOG_LINE_TRACE("Profile Function: {}", name)
#else
#define GRAVLib_PROFILE_SCOPE_LINE2(name, line) GRAVLib::Debug::instrumentorStopwatch GRAVLib_ANONYMOUS_VARIABLE(stopwatch) (name);
#endif
#define GRAVLib_PROFILE_SCOPE_LINE(name, line) GRAVLib_PROFILE_SCOPE_LINE2(name, line)
#define GRAVLib_PROFILE_SCOPE(name) GRAVLib_PROFILE_SCOPE_LINE(name, __LINE__)
#define GRAVLib_PROFILE_FUNCTION() GRAVLib_PROFILE_SCOPE(GRAVLib_CLEAN_FUNC_SIG)
#else
#define GRAVLib_PROFILE_START_SESSION(name, filepath)
#define GRAVLib_PROFILE_END_SESSION()
#define GRAVLib_PROFILE_SCOPE(name)
#define GRAVLib_PROFILE_FUNCTION()
#endif
