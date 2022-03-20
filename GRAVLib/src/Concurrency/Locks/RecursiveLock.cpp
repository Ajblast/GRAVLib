#include "Concurrency/Threads/Thread.h"
#include "Concurrency/Locks/RecursiveLock.h"
#include <thread>

GRAVLib::Concurrency::Locks::recursiveLock::recursiveLock() :
	m_atomic(Threads::threadID_t()), m_refCount(0)
{
}
void GRAVLib::Concurrency::Locks::recursiveLock::lock()
{
	Threads::threadID_t tid = Threads::thread::getCurrentThreadID().m_ThreadID;

	// If this thread doesn't already hold the lock, spin wait until lock acquired
	if (m_atomic.load(std::memory_order_acquire) != tid)
	{
		Threads::threadID_t unlockValue = Threads::threadID_t();

		// Use the weak version because a spurious failure is fine. Not much was actually done
		while (m_atomic.compare_exchange_weak(unlockValue, tid, std::memory_order_release, std::memory_order_acquire) == false)
		{
			unlockValue = Threads::threadID_t();
			std::this_thread::yield();
		}
	}

	// Increment reference count so we can verify that lock and unlock are called in pairs
	m_refCount.fetch_add(1, std::memory_order_acq_rel);
}
void GRAVLib::Concurrency::Locks::recursiveLock::unlock()
{
	// Check if the thread that is unlocking is the actual thread that is allowed to unlock it
	GRAV_ASSERT_EQ(m_atomic.load(std::memory_order_acquire), Threads::thread::getCurrentThreadID().m_ThreadID);


	// Decrease the reference count
	m_refCount.fetch_sub(1, std::memory_order_acq_rel);
	if (m_refCount.load(std::memory_order_acquire) == 0)
		m_atomic.store(Threads::threadID_t(), std::memory_order_release);

}

bool GRAVLib::Concurrency::Locks::recursiveLock::try_lock()
{
	Threads::threadID_t tid = Threads::thread::getCurrentThreadID().m_ThreadID;
	bool acquired = false;

	if (m_atomic.load(std::memory_order_acquire) == tid)
		acquired = true;
	else
	{
		Threads::threadID_t unlockValue = Threads::threadID_t();
		acquired = m_atomic.compare_exchange_strong(unlockValue, tid, std::memory_order_release, std::memory_order_acquire);
	}

	if (acquired)
		m_refCount.fetch_add(1, std::memory_order_acq_rel);

	return acquired;
}
