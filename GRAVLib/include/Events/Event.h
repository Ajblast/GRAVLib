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

namespace GRAVLib::Events
{
	template<typename... T>
	using callback = std::function<bool(T...)>;
	
	///Storage used for an event
	template<typename...  T>
	struct eventStorage
	{
		callback<T...> m_CallbackFunction;
		eventHandle m_Handle;
	};


	template<typename... T>
	class GRAVLibAPI event
	{
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

		eventHandle registerCallback(const callback<bool, T...>& function)
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
			auto it = std::find_if(m_Events.begin(), m_Events.end(), [handle](eventStorage<bool, T...> val) { return val.m_Handle == handle; });

			if (it == m_Events.end())
				return;

			// Erase the callback
			m_Events.erase(it);
		}

		void execute(T... args)
		{
			Locks::scopedLock<decltype(m_VectorLock)> lock(m_VectorLock);

			size_t eventCount = m_Events.size();

			// Don't do anything if there are no events
			if (eventCount == 0)
				return;

			bool handled = false;
			for (size_t i = 0; i < m_Events.size(); i++)
				handled = m_Events[i].m_CallbackFunction(handled, args...);
			
		}
		void clear()
		{
			Locks::scopedLock<decltype(m_VectorLock)> lock(m_VectorLock);

			m_Events.clear();
		}

	private:
		GRAVLib::ref<IEventHandleGenerator> m_HandleGenerator;
		std::vector<eventStorage<bool, T...>> m_Events;

		Locks::spinLock m_VectorLock;
	};
}