#pragma once

#include <chrono>

#include "GRAVLibCore.h" 
#include "Durations.h"
#include "Timestep.h"

namespace GRAVLib::Time
{
	class GRAVLibAPI stopwatch
	{
	public:
		stopwatch();
		stopwatch(const stopwatch& other);
		stopwatch(stopwatch&& other) noexcept;

		stopwatch& operator=(const stopwatch& other);
		stopwatch& operator=(stopwatch&& other) noexcept;

		~stopwatch() = default;

		void start() noexcept;
		void pause() noexcept;
		void reset() noexcept;

		timestep deltaTime() const noexcept;

		inline bool isRunning() { return m_Running; }
	private:
		timePoint m_StartTick;
		nanoseconds m_StoredDuration;

		bool m_Running;
	};
}