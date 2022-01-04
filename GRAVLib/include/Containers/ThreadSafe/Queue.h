#pragma once

#include "GRAVLibCore.h"
#include "Locks/MultiLock.h"
#include "Locks/ScopedLock.h"
#include "Locks/SpinLock.h"
#include <queue>

namespace GRAVLib::Containers::ThreadSafe
{
	// A thread safe queue
	template<typename T, typename L = GRAVLib::Locks::spinLock>
	class GRAVLibAPI queue
	{
	public:
		using dataType = T;
		using lockType = L;

		using multiLockType = Locks::multiLock<lockType, lockType>;
	public:
		queue() {}
		queue(queue& other)
		{
			// Lock the other queue as we are accessing its data
			GRAVLib::Locks::scopedLock<lockType> lock(other.m_Lock);

			// Copy of the items in the other queue
			m_Queue = std::queue<dataType>(other.m_Queue);
		}
		queue(queue&& other) noexcept
		{
			*this = std::move(other);
		}
		queue& operator=(const queue& other)
		{
			if (this != &other)
			{
				multiLockType lock = multiLockType(m_Lock, other.m_Lock);
				Locks::scopedLock<decltype(lock)> f(lock);

				m_Queue = other.m_Queue;
			}

			return *this;
		}
		queue& operator=(queue&& other) noexcept
		{
			if (this != &other)
			{
				multiLockType lock = multiLockType(m_Lock, other.m_Lock);
				Locks::scopedLock<decltype(lock)> f(lock);

				m_Queue = std::move(other.m_Queue);
			}

			return *this;
		}

		~queue() {}

		void enqueue(const dataType& data)
		{
			GRAVLib::Locks::scopedLock<lockType> lock(m_Lock);

			// Push the data
			m_Queue.push(data);
		}
		void enqueue(dataType&& data)
		{
			GRAVLib::Locks::scopedLock<lockType> lock(m_Lock);

			// Push the data
			m_Queue.push(std::move(data));
		}
		// Dequeue data item and store it into data. Undefined if queue is empty
		void dequeue(dataType& data)
		{
			GRAVLib::Locks::scopedLock<lockType> lock(m_Lock);

			// Copy the front of the queue
			data = std::move(m_Queue.front());

			m_Queue.pop();
		}

		inline const bool empty() const { return m_Queue.empty(); }
		inline const size_t size() const { return m_Queue.size(); }

		template<typename T, typename L>
		friend bool operator<(const queue<T, L>& lhs, const queue<T, L>& rhs);
		template<typename T, typename L>
		friend bool operator==(const queue<T, L>& lhs, const queue<T, L>& rhs);
	private:
		mutable lockType m_Lock;		// Lock is marked as mutable because it is changable in const environments
		std::queue<dataType> m_Queue;
	};


	template<typename T, typename L = GRAVLib::Locks::spinLock>
	GRAVLibAPI bool operator==(const queue<T, L>& lhs, const queue<T, L>& rhs)
	{
		Locks::multiLock<L, L> lock = Locks::multiLock<L, L>(lhs.m_Lock, rhs.m_Lock);
		Locks::scopedLock<decltype(lock)> f(lock);

		return lhs.m_Queue == rhs.m_Queue;
	}
	template<typename T, typename L = GRAVLib::Locks::spinLock>
	GRAVLibAPI bool operator!=(const queue<T, L>& lhs, const queue<T, L>& rhs)
	{
		return !(lhs == rhs);
	}

	template<typename T, typename L = GRAVLib::Locks::spinLock>
	GRAVLibAPI bool operator<(const queue<T, L>& lhs, const queue<T, L>& rhs)
	{
		Locks::multiLock<L, L> lock = Locks::multiLock<L, L> (lhs.m_Lock, rhs.m_Lock);
		Locks::scopedLock<decltype(lock)> f(lock);

		return lhs.m_Queue < rhs.m_Queue;
	}
	template<typename T, typename L = GRAVLib::Locks::spinLock>
	GRAVLibAPI bool operator<=(const queue<T, L>& lhs, const queue<T, L>& rhs)
	{
		return !(lhs > rhs);
	}
	template<typename T, typename L = GRAVLib::Locks::spinLock>
	GRAVLibAPI bool operator>(const queue<T, L>& lhs, const queue<T, L>& rhs)
	{
		return rhs < lhs;
	}
	template<typename T, typename L = GRAVLib::Locks::spinLock>
	GRAVLibAPI bool operator>=(const queue<T, L>& lhs, const queue<T, L>& rhs)
	{
		return !(lhs < rhs);
	}
}