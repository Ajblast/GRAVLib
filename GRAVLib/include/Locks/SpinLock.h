#pragma once

#include "GRAVLibCore.h"
#include <atomic>

namespace GRAVLib::Locks
{
	class GRAVLibAPI spinLock
	{
	public:
		spinLock();
		spinLock(const spinLock& other) = delete;
		spinLock(spinLock&& other) noexcept = delete;
		spinLock& operator=(const spinLock& other) = delete;
		spinLock& operator=(spinLock&& other) noexcept = delete;
		~spinLock();

		// Try acquire the lock
		bool try_lock();

		// Acquire the lock
		void lock();
		// Release the lock
		void unlock();
	private:
		std::atomic_flag m_Atomic;

	};
}