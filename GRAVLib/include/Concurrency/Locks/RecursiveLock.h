#pragma once

#include "GRAVLibCore.h"
#include "Concurrency/Threads/ThreadTypes.h"
#include <atomic>

namespace GRAVLib::Concurrency::Locks
{
	// TODO: Convert this lock into an adaptive mutex by making it spin lock for so long before using the system call of std::lock

	// Lock that can be locked again as long as the same thread is acquiring the lock
	class GRAVLibAPI recursiveLock
	{
	public:
		recursiveLock();

		// Acquire the lock
		void lock();
		// Release the lock
		void unlock();

		// attempt to acquire the lock
		bool try_lock();
	private:
		std::atomic<Threads::threadID_t> m_atomic;
		std::atomic<int32> m_refCount;
	};
}