#pragma once

#include "GRAVLibCore.h"
#include "Locks/ScopedLock.h"
#include "Locks/SpinLock.h"
#include "Locks/MultiLock.h"
#include "EventHandle.h"
#include "IEventHandleGenerator.h"

#include <functional>
#include <mutex>
#include <algorithm>

#define GRAV_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) {return this->fn(std::forward<decltype(args)>(args)...); }

namespace GRAVLib::Events
{
	template<typename... T>
	using eventCallback = std::function<bool(T...)>;
	
	///Storage used for an event
	template<typename...  T>
	struct eventStorage
	{
		eventCallback<T...> m_CallbackFunction;
		eventHandle m_Handle;
	};


	template<typename... T>
	class GRAVLibAPI event
	{
	public:
		using callback = eventCallback<bool, T...>;
		using storage = eventStorage<bool, T...>;
	public:
		event()	{}
		event(GRAVLib::ref<IEventHandleGenerator> handleGenerator) : m_HandleGenerator(std::move(handleGenerator)) {}
		event(const event& other) = delete;
		event(event&& other) noexcept
		{
			*this = std::move(other);
		}
		event& operator=(const event& other) = delete;
		event& operator=(event&& other) noexcept
		{
			if (this != &other)
			{
				Locks::multiLock<Locks::spinLock, Locks::spinLock> f = Locks::multiLock<Locks::spinLock, Locks::spinLock>(m_VectorLock, other.m_VectorLock);
				Locks::scopedLock<decltype(f)> lock(f);

				m_HandleGenerator = std::move(other.m_HandleGenerator);
				m_Events = std::move(other.m_Events);

				// Don't move the actual locks. Let a new lock be made because who cares, locks shouldn't hold internal state like that
			}
		}
		~event() {}

		eventHandle registerCallback(const callback& function)
		{
			Locks::scopedLock<decltype(m_VectorLock)> lock(m_VectorLock);

			// Get the new handle
			eventHandle handle = m_HandleGenerator->generate();

			// Add the function to the list of events with the handle for it
			m_Events.push_back({ function, handle });

			return handle;
		}
		void unregisterCallback(eventHandle handle)
		{
			Locks::scopedLock<decltype(m_VectorLock)> lock(m_VectorLock);

			// Find the event by the handle value
			auto it = std::find_if(m_Events.begin(), m_Events.end(), [handle](storage val) { return val.m_Handle == handle; });

			if (it == m_Events.end())
				return;

			// Erase the callback
			m_Events.erase(it);
		}

		bool execute(T... args)
		{
			Locks::scopedLock<decltype(m_VectorLock)> lock(m_VectorLock);

			size_t eventCount = m_Events.size();

			// Don't do anything if there are no events
			if (eventCount == 0)
				return false;

			bool handled = false;
			for (size_t i = 0; i < m_Events.size(); i++)
				handled = m_Events[i].m_CallbackFunction(handled, args...);
			
			return handled;
		}

		inline void clear()
		{
			Locks::scopedLock<decltype(m_VectorLock)> lock(m_VectorLock);

			m_Events.clear();
		}
		inline const size_t size()
		{
			Locks::scopedLock<decltype(m_VectorLock)> lock(m_VectorLock);

			return m_Events.size();
		}
	private:
		GRAVLib::ref<IEventHandleGenerator> m_HandleGenerator;
		std::vector<storage> m_Events;

		Locks::spinLock m_VectorLock;
	};
}