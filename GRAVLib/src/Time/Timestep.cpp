#include "GRAVLib/Time\Timestep.h"

GRAVLib::Time::timestep::timestep() : m_Time(0)
{
}
GRAVLib::Time::timestep::timestep(const timestep& other) : m_Time(other.m_Time)
{
}
GRAVLib::Time::timestep::timestep(timestep&& other) noexcept
{
	*this = std::move(other);
}
GRAVLib::Time::timestep& GRAVLib::Time::timestep::operator=(const timestep& other)
{
	if (this != &other)
	{
		m_Time = other.m_Time;
	}

	return *this;
}
GRAVLib::Time::timestep& GRAVLib::Time::timestep::operator=(timestep&& other) noexcept
{
	if (this != &other)
	{
		m_Time = std::move(other.m_Time);
	}

	return *this;
}

GRAVLib::Time::timestep::timestep(timePoint startTick, timePoint endTick) : m_Time(endTick - startTick)
{}
GRAVLib::Time::timestep::timestep(timeDurationCount time) : m_Time(time)
{}
GRAVLib::Time::timestep::timestep(float time) : m_Time(std::chrono::duration_cast<Time::nanoseconds>(Time::seconds(time)))
{}

GRAVLib::Time::timestep::operator double() const noexcept
{
	return seconds();
}
GRAVLib::Time::timeDurationCount GRAVLib::Time::timestep::nanoseconds() const noexcept
{
	return elapsedNanosecondsDuration().count();
}
double GRAVLib::Time::timestep::microseconds() const noexcept
{
	return elapsedMicrosecondsDuration().count();
}
double GRAVLib::Time::timestep::milliseconds() const noexcept
{
	return elapsedMillisecondsDuration().count();
}
double GRAVLib::Time::timestep::seconds() const noexcept
{
	return elapsedSecondsDuration().count();
}
double GRAVLib::Time::timestep::minutes() const noexcept
{
	return elapsedMinutesDuration().count();
}
double GRAVLib::Time::timestep::hours() const noexcept
{
	return elapsedHoursDuration().count();
}

GRAVLib::Time::nanoseconds GRAVLib::Time::timestep::elapsedNanosecondsDuration() const noexcept
{
	return m_Time;
}
GRAVLib::Time::microseconds GRAVLib::Time::timestep::elapsedMicrosecondsDuration() const noexcept
{
	return std::chrono::duration_cast<Time::microseconds>(elapsedNanosecondsDuration());
}
GRAVLib::Time::milliseconds GRAVLib::Time::timestep::elapsedMillisecondsDuration() const noexcept
{
	return std::chrono::duration_cast<Time::milliseconds > (elapsedNanosecondsDuration());
}
GRAVLib::Time::seconds GRAVLib::Time::timestep::elapsedSecondsDuration() const noexcept
{
	return std::chrono::duration_cast<Time::seconds>(elapsedNanosecondsDuration());
}
GRAVLib::Time::minutes GRAVLib::Time::timestep::elapsedMinutesDuration() const noexcept
{
	return std::chrono::duration_cast<Time::minutes>(elapsedNanosecondsDuration());
}
GRAVLib::Time::hours GRAVLib::Time::timestep::elapsedHoursDuration() const noexcept
{
	return std::chrono::duration_cast<Time::hours>(elapsedNanosecondsDuration());
}
