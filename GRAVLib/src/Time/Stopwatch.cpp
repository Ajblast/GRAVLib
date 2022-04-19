#include "Time\Stopwatch.h"

GRAVLib::Time::stopwatch::stopwatch() : m_StartTick(std::chrono::steady_clock::now()), m_StoredDuration(0), m_Running(false)
{
}
GRAVLib::Time::stopwatch::stopwatch(const stopwatch& other) : m_StartTick(other.m_StartTick), m_Running(other.m_Running), m_StoredDuration(other.m_StoredDuration)
{
}
GRAVLib::Time::stopwatch::stopwatch(stopwatch&& other) noexcept : m_StartTick(std::move(other.m_StartTick)), m_Running(std::move(other.m_Running)), m_StoredDuration(std::move(other.m_StoredDuration))
{
}
GRAVLib::Time::stopwatch& GRAVLib::Time::stopwatch::operator=(const stopwatch& other)
{
	m_StartTick = other.m_StartTick;
	m_Running = other.m_Running;
	m_StoredDuration = other.m_StoredDuration;

	return *this;
}
GRAVLib::Time::stopwatch& GRAVLib::Time::stopwatch::operator=(stopwatch&& other) noexcept
{
	if (this != &other)
	{
		m_StartTick = std::move(other.m_StartTick);
		m_Running = std::move(other.m_Running);
		m_StoredDuration = std::move(other.m_StoredDuration);
	}

	return *this;
}

void GRAVLib::Time::stopwatch::start() noexcept
{
	// Do nothing if it is currently running
	if (m_Running)
		return;

	// Get the current time
	m_StartTick = std::chrono::steady_clock::now();

	m_Running = true;
}
void GRAVLib::Time::stopwatch::pause() noexcept
{
	// Do nothing if it is not currently running
	if (!m_Running)
		return;

	// Get the current duration
	nanoseconds currentDuration = std::chrono::duration_cast<nanoseconds>(std::chrono::steady_clock::now() - m_StartTick);

	// Add the curernt duration to the stored duration
	m_StoredDuration += currentDuration;

	m_Running = false;
}
void GRAVLib::Time::stopwatch::reset() noexcept
{
	// Reset the stored duration
	m_StoredDuration = nanoseconds(0);

	// Set the starting tick
	m_StartTick = std::chrono::steady_clock::now();

	m_Running = false;
}

GRAVLib::Time::timestep GRAVLib::Time::stopwatch::deltaTime() const noexcept
{
	// Get the current duration
	nanoseconds elapsedDuration = m_StoredDuration;

	// Calculate the current duration if the timer is running
	if (m_Running)
	{
		// Add to the duration if the stopwatch is running
		elapsedDuration += (std::chrono::steady_clock::now() - m_StartTick);
	}

	return timestep(elapsedDuration.count());
}
