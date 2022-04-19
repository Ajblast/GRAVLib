#pragma once

#include "GRAVLib/GRAVLibCore.h"
#include "GRAVLib/Concurrency/Jobs/JobPriority.h"

#include <functional>

#define GRAVLib_JOB_ENTRY_POINT(jobName) void jobName(uintptr_t param)

namespace GRAVLib::Concurrency::Jobs
{
	typedef std::function<void(uintptr_t)> entryPoint;	// A job's entry point

	// A job declaration
	struct GRAVLibAPI declaration
	{
	public:
		declaration();
		declaration(entryPoint entryPoint, uintptr_t arg, jobPriority priority = jobPriority::NORMAL);
		declaration(const declaration& other) = default;
		declaration(declaration&& other) = default;
		declaration& operator=(const declaration& other) = default;
		declaration& operator=(declaration&& other) = default;
		~declaration() = default;
	public:
		entryPoint m_EntryPoint;	// The job function pointer
		uintptr_t m_Arg;			// Pointer to the job's arguments
		jobPriority m_Priority;		// The priority of the job
	};
}