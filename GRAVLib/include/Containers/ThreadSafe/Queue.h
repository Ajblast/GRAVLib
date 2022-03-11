#pragma once

#include "GRAVLibCore.h"
#include <queue>

namespace GRAVLib::Containers::ThreadSafe
{
	// A thread safe queue
	template<typename T>
	class GRAVLibAPI queue
	{
	public:
		using dataType = T;

	public:
		queue();
		queue(const queue& other);
		queue(queue&& other) noexcept;
		queue& operator=(const queue& other);
		queue& operator=(queue&& other) noexcept;
		~queue();

		// Enqueue data
		void enqueue(const dataType& data);
		// Enqueue data
		void enqueue(dataType&& data);
		// Dequeue data item and store it into data. Undefined if queue is empty
		void dequeue(dataType& data);

		const bool empty() const;
		const size_t size() const;

		template<typename T>
		friend bool operator<(const queue<T>& lhs, const queue<T>& rhs);
		template<typename T>
		friend bool operator==(const queue<T>& lhs, const queue<T>& rhs);
	private:
		GRAVLib_AUTO_MUTEX;

		std::queue<dataType> m_Queue;
	};

	#pragma region Constructors
	template<typename T>
	queue<T>::queue()
	{}
	template<typename T>
	queue<T>::queue(const queue& other) 
	{
		GRAVLib_LOCK_LOCKS(GRAVLib_AUTO_MUTEX_NAME, other.GRAVLib_AUTO_MUTEX_NAME);

		// Copy of the items in the other queue
		m_Queue = std::queue<dataType>(other.m_Queue);
	}
	template<typename T>
	queue<T>::queue(queue&& other) noexcept
	{
		*this = std::move(other);
	}
	template<typename T>
	queue<T>& queue<T>::operator=(const queue& other)
	{
		if (this != &other)
		{
			GRAVLib_LOCK_LOCKS(GRAVLib_AUTO_MUTEX_NAME, other.GRAVLib_AUTO_MUTEX_NAME);

			m_Queue = other.m_Queue;
		}

		return *this;
	}
	template<typename T>
	queue<T>& queue<T>::operator=(queue&& other) noexcept
	{
		if (this != &other)
		{
			GRAVLib_LOCK_LOCKS(GRAVLib_AUTO_MUTEX_NAME, other.GRAVLib_AUTO_MUTEX_NAME);

			m_Queue = std::move(other.m_Queue);
		}

		return *this;
	}
	template<typename T>
	queue<T>::~queue()
	{}
	#pragma endregion

	template<typename T>
	inline void queue<T>::enqueue(const dataType& data)
	{
		GRAVLib_LOCK_AUTO_MUTEX;

		// Push the data
		m_Queue.push(data);
	}
	template<typename T>
	inline void queue<T>::enqueue(dataType&& data)
	{
		GRAVLib_LOCK_AUTO_MUTEX;

		// Push the data
		m_Queue.push(std::move(data));
	}
	template<typename T>
	inline void queue<T>::dequeue(dataType& data)
	{
		GRAVLib_LOCK_AUTO_MUTEX;

		// Copy the front of the queue
		data = std::move(m_Queue.front());

		m_Queue.pop();
	}

	template<typename T>
	inline const bool queue<T>::empty() const
	{
		GRAVLib_LOCK_AUTO_MUTEX;

		return m_Queue.empty();
	}
	template<typename T>
	inline const size_t queue<T>::size() const
	{
		GRAVLib_LOCK_AUTO_MUTEX;

		return m_Queue.size();
	}



	template<typename T>
	GRAVLibAPI bool operator==(const queue<T>& lhs, const queue<T>& rhs)
	{
		GRAVLib_LOCK_LOCKS(lhs.GRAVLib_AUTO_MUTEX_NAME, rhs.GRAVLib_AUTO_MUTEX_NAME);

		return lhs.m_Queue == rhs.m_Queue;
	}
	template<typename T>
	GRAVLibAPI bool operator!=(const queue<T>& lhs, const queue<T>& rhs)
	{
		return !(lhs == rhs);
	}

	template<typename T>
	GRAVLibAPI bool operator<(const queue<T>& lhs, const queue<T>& rhs)
	{
		GRAVLib_LOCK_LOCKS(lhs.GRAVLib_AUTO_MUTEX_NAME, rhs.GRAVLib_AUTO_MUTEX_NAME);

		return lhs.m_Queue < rhs.m_Queue;
	}
	template<typename T>
	GRAVLibAPI bool operator<=(const queue<T>& lhs, const queue<T>& rhs)
	{
		return !(lhs > rhs);
	}
	template<typename T>
	GRAVLibAPI bool operator>(const queue<T>& lhs, const queue<T>& rhs)
	{
		return rhs < lhs;
	}
	template<typename T>
	GRAVLibAPI bool operator>=(const queue<T>& lhs, const queue<T>& rhs)
	{
		return !(lhs < rhs);
	}
}