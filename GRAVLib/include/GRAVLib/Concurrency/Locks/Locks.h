#pragma once

#define GRAVLib_AUTO_MUTEX_NAME m_GRAVLibMutex
#if GRAVLib_THREAD_LIBRARY == GRAVLib_THREAD_LIBRARY_NONE
	#include "GRAVLib/Concurrency/Locks/Defines/LocksNone.h"
#elif GRAVLib_THREAD_LIBRARY == GRAVLib_THREAD_LIBRARY_STD
	#include "GRAVLib/Concurrency/LocksDefines/LocksSTD.h"
#elif GRAVLib_THREAD_LIBRARY == GRAVLib_THREAD_LIBRARY_GRAV
	#include "GRAVLib/Concurrency/Locks/Defines/LocksGRAV.h"
#else
	// Default to std locks
	#include "GRAVLib/Concurrency/Locks/Defines/LocksGRAV.h"
#endif

// The GRAVLib mutex is a simple recursive mutex that should not be used in jobs. This mutex is fine as long as the lock being tied to a thread is okay.
#define GRAVLib_MUTEX(name) GRAVLib_MUTEX_IMPL(name)
#define GRAVLib_LOCK_MUTEX(name) GRAVLib_LOCK_MUTEX_IMPL(name)
#define GRAVLib_LOCK_NAMED_MUTEX(mutexName, lockName) GRAVLib_LOCK_NAMED_MUTEX_IMPL(mutexName, lockName)
#define GRAVLib_LOCK_LOCKS(...) GRAVLib_LOCK_LOCKS_IMPL(__VA_ARGS__)

// The GRAVLib auto mutex is the same as the GRAVLib mutex and must follow all of the same warnings.
#define GRAVLib_AUTO_MUTEX GRAVLib_AUTO_MUTEX_IMPL
#define GRAVLib_LOCK_AUTO_MUTEX GRAVLib_LOCK_AUTO_MUTEX_IMPL


// The GRAVLib job mutex is a mutex that can be used within a job
#define GRAVLib_JOB_MUTEX(name) throw
// TODO: Implement job lock