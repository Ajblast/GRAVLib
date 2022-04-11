#pragma once

#include "GRAVLibCore.h"
#include <mutex>
#include <tuple>

namespace GRAVLib::Concurrency::Locks
{
	// A multilock is a RAII lock that is able to hold a lock for multiple locks at a time. 
	template<class... L>
	class multiLock
	{
	public:
		explicit multiLock(L&... locks);
		multiLock(const multiLock& other) = delete;
		multiLock(multiLock&& other) = delete;
		~multiLock();

	private:
		std::tuple<L&...> m_Locks;
	};

	template<class... L>
	inline multiLock<L...>::multiLock(L&... locks) : m_Locks(locks...)
	{
		// Lock each of the locks
		std::lock(locks...);
	}
	template<class... L>
	inline multiLock<L...>::~multiLock()
	{
		// For every lock, unlock it
		std::apply([](L&... locks) {(..., (void)locks.unlock()); }, m_Locks);
	}


	// Specialization of a multilock that only has a single lock
	template<class L>
	class multiLock<L>
	{
	public:
		explicit multiLock(L& lock);// : m_Lock(lock) { m_Lock.lock(); }
		multiLock(const multiLock& other) = delete;
		multiLock(multiLock&& other) = delete;
		~multiLock();

	private:
		L& m_Lock;
	};

	template<class L>
	inline multiLock<L>::multiLock(L& lock) : m_Lock(lock)
	{
		m_Lock.lock();
	}
	template<class L>
	inline multiLock<L>::~multiLock()
	{
		m_Lock.unlock();
	}


	// Specialization of a multilock that has no lock
	template<>
	class GRAVLibAPI multiLock<>
	{
	public:
		explicit multiLock() {}
		multiLock(const multiLock& other) = delete;
		multiLock(multiLock&& other) = delete;
		~multiLock() {}
	};
}