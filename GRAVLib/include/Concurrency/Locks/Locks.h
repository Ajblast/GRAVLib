#pragma once

#define GRAVLib_THREAD_LIBRARY_NONE 0
#define GRAVLib_THREAD_LIBRARY_STD 1

#define GRAVLib_AUTO_MUTEX_NAME m_GRAVLibMutex
#if GRAVLib_THREAD_LIBRARY == GRAVLib_THREAD_LIBRARY_NONE
	#include "LocksNone.h"
#elif GRAVLib_THREAD_LIBRARY == GRAVLib_THREAD_LIBRARY_STD
	#include "LocksSTD.h"
#else
	// Default to std locks
	#include "LocksSTD.h"
#endif


#define GRAVLib_MUTEX(name) GRAVLib_MUTEX_IMPL(name)
#define GRAVLib_LOCK_MUTEX(name) GRAVLib_LOCK_MUTEX_IMPL(name)
#define GRAVLib_LOCK_NAMED_MUTEX(mutexName, lockName) GRAVLib_LOCK_NAMED_MUTEX_IMPL(mutexName, lockName)
#define GRAVLib_LOCK_LOCKS(...) GRAVLib_LOCK_LOCKS_IMPL(__VA_ARGS__)

#define GRAVLib_AUTO_MUTEX GRAVLib_AUTO_MUTEX_IMPL
#define GRAVLib_LOCK_AUTO_MUTEX GRAVLib_LOCK_AUTO_MUTEX_IMPL