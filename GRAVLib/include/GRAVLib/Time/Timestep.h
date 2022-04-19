#pragma once

#include "GRAVLib/GRAVLibCore.h"
#include "GRAVLib/Time/Durations.h"

namespace GRAVLib::Time
{
	// Simple representation of a step in time
	class GRAVLibAPI timestep
	{
	public:
		// Clock start and stop points
		timestep(timePoint startTick, timePoint endTick);
		// The amount of time in nanoseconds
		timestep(timeDurationCount time);
		// The amount of time in seconds
		timestep(float time);

		timestep();
		timestep(const timestep& other);
		timestep(timestep&& other) noexcept;
		timestep& operator=(const timestep& other);
		timestep& operator=(timestep&& other) noexcept;


		explicit operator double() const noexcept;
		timeDurationCount nanoseconds() const noexcept;
		double microseconds() const noexcept;
		double milliseconds() const noexcept;
		double seconds() const noexcept;
		double minutes() const noexcept;
		double hours() const noexcept;

		Time::nanoseconds elapsedNanosecondsDuration() const noexcept;
		Time::microseconds elapsedMicrosecondsDuration() const noexcept;
		Time::milliseconds elapsedMillisecondsDuration() const noexcept;
		Time::seconds elapsedSecondsDuration() const noexcept;
		Time::minutes elapsedMinutesDuration() const noexcept;
		Time::hours elapsedHoursDuration() const noexcept;
	private:
		// The elapsed duration in nanoseconds
		Time::nanoseconds m_Time;
	};
}