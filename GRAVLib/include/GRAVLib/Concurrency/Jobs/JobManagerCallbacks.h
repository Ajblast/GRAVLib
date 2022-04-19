#pragma once

#include "GRAVLib/GRAVLibCore.h"
#include "GRAVLib/Concurrency/Threads/ThreadID.h"
#include "GRAVLib/Concurrency/Fibers/FiberID.h"
#include "GRAVLib/Concurrency/Jobs/Declaration.h"
#include <functional>

namespace GRAVLib::Concurrency::Jobs
{
	typedef std::function<void()> jobManagerCallback;													// Called when the job manager starts up and shuts down

	typedef std::function<void(Threads::threadIndex_t threadsCreated)> threadCreationCallback;			// Called after threads are created
	typedef std::function<void(Fibers::fiberIndex_t fibersCreated)> fiberCreationCallback;				// Called after fibers are created

	typedef std::function<void(Threads::threadID threadID)> threadCallback;								// Called when a thread does something
	typedef std::function<void(Fibers::fiberID fiberID)> fiberCallback;									// Called when a fiber does something

	typedef std::function<void(Fibers::fiberID fiberIndex)> fiberAttachedCallback;						// Called when a fiber is attached
	typedef std::function<void(Fibers::fiberID fiberIndex, bool interupted)> fiberDetachedCallback;		// Called when a fiber detaches. Was the fiber interrupted because of a wait

	typedef std::function<void(declaration job)> jobCallback;											// Called when a job does something

	// The callbacks a job manager will run
	struct GRAVLibAPI jobManagerCallbacks
	{
	public:
		jobManagerCallback m_OnJobManagerStartup;	// Called after the job manager starts up
		jobManagerCallback m_OnJobManagerShutdown;	// Called after the job manager shuts down

		threadCreationCallback m_OnThreadsCreated;	// Called after threads are created
		fiberCreationCallback m_OnFibersCreated;	// Called after fibers are created

		threadCallback m_OnThreadInitialized;		// Called when a thread starts initialization
		threadCallback m_OnThreadEnds;				// Called when a thread ends

		fiberAttachedCallback m_OnFiberAttached;	// Called when a fiber is attached
		fiberDetachedCallback m_OnFiberDetached;	// Called when a fiber is detached

		fiberCallback m_OnFiberInitialized;			// Called when a fiber starts initialization
		fiberCallback m_OnFiberEnds;				// Called when a fiber ends

		jobCallback m_OnJobBefore;					// Called before a job is executed
		jobCallback m_OnJobAfter;					// Called after a job is executed
	};
}