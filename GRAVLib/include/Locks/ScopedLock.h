#pragma once

#include "GRAVLibCore.h"

namespace GRAVLib::Locks
{
	template<class L>
	class GRAVLibAPI scopedLock
	{
	public:
		explicit scopedLock(L& lock) : m_pLock(&lock)
		{
			m_pLock->lock();
		}
		scopedLock(const scopedLock& other) = delete;
		scopedLock(scopedLock&& other) noexcept = delete;
		scopedLock& operator=(const scopedLock& other) = delete;
		scopedLock& operator=(scopedLock&& other) noexcept = delete;

		~scopedLock()
		{
			m_pLock->unlock();
		}

	private:
		L* m_pLock;
	};
}