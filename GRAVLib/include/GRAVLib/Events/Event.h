#pragma once

#include "GRAVLib/GRAVLibCore.h"
#include "GRAVLib/Concurrency/Locks/Locks.h"
#include "GRAVLib/Events//EventHandle.h"
#include "GRAVLib/Events/IEventHandleGenerator.h"

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
		event();
		event(GRAVLib::ref<IEventHandleGenerator> handleGenerator);
		event(const event& other) = delete;
		event(event&& other) noexcept;
		event& operator=(const event& other) = delete;
		event& operator=(event&& other) noexcept;
		~event();

		eventHandle registerCallback(const callback& function);
		void unregisterCallback(eventHandle handle);

		bool execute(T... args);

		void clear() const;
		const size_t size() const;
	private:
		GRAVLib_AUTO_MUTEX;

		GRAVLib::ref<IEventHandleGenerator> m_HandleGenerator;
		std::vector<storage> m_Events;
	};

	#pragma region Constructors
	template<typename... T>
	inline event<T...>::event()
	{}
	template<typename... T>
	inline event<T...>::event(GRAVLib::ref<IEventHandleGenerator> handleGenerator) :
		m_HandleGenerator(handleGenerator)
	{}
	template<typename... T>
	inline event<T...>::event(event&& other) noexcept
	{
		*this = std::move(other);
	}
	template<typename... T>
	inline event<T...>& event<T...>::operator=(event&& other) noexcept
	{
		if (this != &other)
		{
			GRAVLib_LOCK_LOCKS(GRAVLib_AUTO_MUTEX_NAME, other.GRAVLib_AUTO_MUTEX_NAME);

			m_HandleGenerator = std::move(other.m_HandleGenerator);
			m_Events = std::move(other.m_Events);
		}

		return *this;
	}
	template<typename... T>
	inline event<T...>::~event()
	{}
	#pragma endregion

	template<typename ...T>
	inline eventHandle event<T...>::registerCallback(const callback& function)
	{
		GRAVLib_LOCK_AUTO_MUTEX;

		// Get the new handle
		eventHandle handle = m_HandleGenerator->generate();

		// Add the function to the list of events with the handle for it
		m_Events.push_back({ function, handle });

		return handle;
	}
	template<typename ...T>
	inline void event<T...>::unregisterCallback(eventHandle handle)
	{
		GRAVLib_LOCK_AUTO_MUTEX;

		// Find the event by the handle value
		auto it = std::find_if(m_Events.begin(), m_Events.end(), [handle](storage val) { return val.m_Handle == handle; });

		if (it == m_Events.end())
			return;

		// Erase the callback
		m_Events.erase(it);
	}

	template<typename ...T>
	inline bool event<T...>::execute(T ...args)
	{
		GRAVLib_LOCK_AUTO_MUTEX;

		// Don't do anything if there are no events
		if (size() == 0)
			return false;

		bool handled = false;
		for (size_t i = 0; i < size(); i++)
			handled = m_Events[i].m_CallbackFunction(handled, args...);

		return handled;
	}

	template<typename ...T>
	inline void event<T...>::clear() const
	{
		GRAVLib_LOCK_AUTO_MUTEX;

		m_Events.clear();
	}
	template<typename ...T>
	inline const size_t event<T...>::size() const
	{
		GRAVLib_LOCK_AUTO_MUTEX;

		return m_Events.size();
	}

}