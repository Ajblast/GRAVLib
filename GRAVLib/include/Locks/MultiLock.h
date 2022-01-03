#pragma once
#include "GRAVLibCore.h"
#include <mutex>
#include <tuple>

namespace GRAVLib::Locks
{
	// A wrapper around a set of mutex locks and will acquire all of the locks at a time
	template<typename... L>
	class GRAVLibAPI multiLock
	{
	public:
		explicit multiLock(L&... locks) : m_Locks(locks...) {}
		multiLock(const multiLock& other) = delete;
		multiLock(multiLock&& other) noexcept = delete;
		multiLock& operator=(const multiLock& other) = delete;
		multiLock& operator=(multiLock&& other) noexcept = delete;
		~multiLock() {}

		// Try acquire all of the locks. Returns which lock failed
		int try_lock()
		{
			return std::apply([](L&... locks) {return std::try_lock(locks...); }, m_Locks);
		}

		// Acquire the lock. Warning, this will not unlock locks if an exception is thrown while locking
		void lock()
		{
			std::apply([](L&... locks) {std::lock(locks...); }, m_Locks);
		}
		// Release the lock. Warning, this will nto unlock locks if an exception is thrown while lock
		void unlock()
		{
			std::apply([](L&... locks) { (..., (void)locks.unlock()); }, m_Locks);
		}
	private:
		std::tuple<L&...> m_Locks;
	};
}