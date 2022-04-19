#pragma once

#include "GRAVLib/GRAVLibCore.h"
#include "GRAVLib/Concurrency/Locks/ScopedLock.h"

namespace GRAVLib::Concurrency::Locks
{
	// An unnecessary lock is a lock meant to determine if a lock is actually needed for a critical section.
	class GRAVLibAPI unnecessaryLock
	{
	public:
		void lock();
		void unlock();

	private:
		// This variable doesn't need to actually be atomic because missing a lock is fine. Just need to make sure not optimizations are done.
		volatile bool m_Locked;
	};

	inline void unnecessaryLock::lock()
	{
		GRAV_ASSERT_FALSE(m_Locked);
		m_Locked = true;
	}
	inline void unnecessaryLock::unlock()
	{
		GRAV_ASSERT_TRUE(m_Locked);
		m_Locked = false;
	}

	using unnecessaryLockJanitor = scopedLock<unnecessaryLock>;
}

#ifdef GRAVLib_ASSERTIONS
	#define GRAVLib_BEGEIN_ASSERT_LOCK_NOT_NECESSARY(L) L.lock();
	#define GRAVLib_END_ASSERT_LOCK_NOT_NECESSARY(L) L.unlock();
	#define GRAVLib_UNNECESSARY_LOCK(L) GRAVLib::Concurrency::Locks::unnecessaryLockJanitor m_TestLock(L);
#else
	#define GRAVLib_BEGEIN_ASSERT_LOCK_NOT_NECESSARY(L)
	#define GRAVLib_END_ASSERT_LOCK_NOT_NECESSARY(L)
	#define GRAVLib_UNNECESSARY_LOCK
#endif