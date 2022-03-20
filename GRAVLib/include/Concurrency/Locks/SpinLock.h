#pragma once

#include "GRAVLibCore.h"
#include <atomic>

namespace GRAVLib::Concurrency::Locks
{
	class GRAVLibAPI spinLock
	{
	public:
		spinLock();
		~spinLock();

		// Try acquire the lock
		bool try_lock();

		// Acquire the lock
		void lock();
		// Release the lock
		void unlock();
	private:
		std::atomic_flag m_atomic = ATOMIC_FLAG_INIT;
	};
}