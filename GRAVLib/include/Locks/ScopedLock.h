#pragma once

#include "GRAVLibCore.h"

namespace GRAVLib::Locks
{
	template<class LOCK>
	class GRAVLibAPI scopedLock
	{
		typedef LOCK lock_t;
		lock_t* m_pLock;

	public:
		explicit scopedLock(lock_t& lock) : m_pLock(&lock)
		{
			m_pLock->lock();
		}
		~scopedLock()
		{
			m_pLock->unlock();
		}
	};
}