#pragma once

#include "GRAVLibCore.h"

namespace GRAVLib::Concurrency::Locks
{
	// A RAII wrapper for any lock that conforms to the BasicLockable requirements
	template<class L>
	class scopedLock
	{
	public:
		explicit scopedLock(L& lock);
		scopedLock(const scopedLock& other) = delete;
		scopedLock(scopedLock&& other) = delete;
		scopedLock& operator=(const scopedLock& other) = delete;
		scopedLock& operator=(scopedLock&& other) = delete;
		~scopedLock();
	private:
		L* m_Lock;
	};

	template<class L>
	inline scopedLock<L>::scopedLock(L& lock) :
		m_Lock(&lock)
	{
		m_Lock->lock();
	}
	template<class L>
	inline scopedLock<L>::~scopedLock()
	{
		m_Lock->unlock();
	}
}