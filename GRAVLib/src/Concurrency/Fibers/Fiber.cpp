#include "Concurrency/Fibers/Fiber.h"
#include "Concurrency/Fibers/Exceptions/FiberCreationException.h"
#include "Concurrency/Fibers/Exceptions/FiberToThreadException.h"
#include "Concurrency/Fibers/Exceptions/FiberSwitchingException.h"
#include "Debug/Logging/Logging.h"

#include <format>

#if GRAVLib_PLATFORM == GRAVLib_PLATFORM_WINDOWS
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

GRAVLib::Concurrency::Fibers::fiber::fiber() : 
	m_IsThreadFiber(false)
{
}
GRAVLib::Concurrency::Fibers::fiber::fiber(fiberCallback callback) : fiber(callback, std::string())
{
}
GRAVLib::Concurrency::Fibers::fiber::fiber(fiberCallback callback, const std::string& name) : 
	m_Name(name), m_IsThreadFiber(false)
{
	fiberHandle_t handle;

	#if GRAVLib_PLATFORM == GRAVLib_PLATFORM_WINDOWS
	// Create the fiber with the launching fiber function
	handle = (fiberHandle_t)CreateFiber(0, (LPFIBER_START_ROUTINE)callback, this);

	//auto target = callback.target<void(fiber*)>();
	//handle = (fiberHandle_t)CreateFiber(0, (LPFIBER_START_ROUTINE)callback.target<void(fiber*)>(), this);
	#endif

	if (handle == NULL)
		throw Exceptions::fiberCreationException("Unable to spawn fiber.");

	m_ID = fiberID(handle);
}
GRAVLib::Concurrency::Fibers::fiber::fiber(fiber&& other) noexcept
{
	*this = std::move(other);
}
GRAVLib::Concurrency::Fibers::fiber& GRAVLib::Concurrency::Fibers::fiber::operator=(fiber&& other) noexcept
{
	if (this != &other)
	{
		GRAV_ASSERT_FALSE(valid()); // The current fiber must not be valid because we would be losing the representation of the internal handle

		if (valid())
			terminate();

		m_ID = std::exchange(other.m_ID, {});
		m_Name = std::exchange(other.m_Name, {});

		m_IsThreadFiber = std::exchange(other.m_IsThreadFiber, {});
	}

	return *this;
}
GRAVLib::Concurrency::Fibers::fiber::~fiber()
{
	GRAV_ASSERT_FALSE(valid()); // The current fiber must not be valid to be destructed

	// It is not an error if a fiber object destructor is called while the fiber is valid. Unlike threads
	if (valid())
	{
		GRAVLib_LOG_LINE_CRITICAL("Attempting to call destructor on still valid fiber [{}].", *this);
		terminate();
	}
}

void GRAVLib::Concurrency::Fibers::fiber::initializeFromCurrentThread()
{
	GRAV_ASSERT_FALSE(valid());

	fiberHandle_t handle;

	// Convert the thread into a fiber
	#if GRAVLib_PLATFORM == GRAVLib_PLATFORM_WINDOWS
	handle = ConvertThreadToFiber(nullptr);
	#endif

	// Was a handle created
	if (handle == NULL)
		throw Exceptions::fiberCreationException("Unable to create fiber from current thread.");

	m_ID = fiberID(handle);
	m_IsThreadFiber = true;
}
void GRAVLib::Concurrency::Fibers::fiber::convertToThread()
{
	GRAV_ASSERT_TRUE(valid());
	GRAV_ASSERT_TRUE(m_IsThreadFiber);	// Fiber must be a thread fiber to convert back to a thread

	bool result;
	#if GRAVLib_PLATFORM == GRAVLib_PLATFORM_WINDOWS
	// Turn this fiber back into a thread. Used for the main thread
	result = ConvertFiberToThread();
	#endif

	// Was the fiber turned back into a thread
	if (result == false)
		throw Exceptions::fiberToThreadException(
			std::format("Unable to convert the current fiber [{}] into a thread.", *this)
		);

	m_IsThreadFiber = false;
	m_ID = fiberID();
}

void GRAVLib::Concurrency::Fibers::fiber::switchTo(fiber& fiber)
{
	GRAV_ASSERT_TRUE(valid());
	GRAV_ASSERT_TRUE(fiber.valid());	// Fiber must have a valid 

	#if GRAVLib_PLATFORM == GRAVLib_PLATFORM_WINDOWS
	try
	{
		SwitchToFiber(fiber.getFiberHandle());
	}
	catch (...)
	{
		throw Exceptions::fiberSwitchingException(
			std::format("Fiber [{}] unable to switch to fiber [{}]", *this, fiber)
		);
	}
	#endif
}

void GRAVLib::Concurrency::Fibers::fiber::close()
{
	GRAV_ASSERT_TRUE(valid());

	if (m_IsThreadFiber)
		throw Exceptions::fiberException(
			std::format("Attempting to close a fiber [{}] that represents a thread. Thread fibers cannot be closed and must instead be converted back to threaeds.", *this)
		);

	// Delete the system fiber
	#if GRAVLib_PLATFORM == GRAVLib_PLATFORM_WINDOWS
	DeleteFiber(getFiberHandle());
	#endif

	// Reset the fiber identifying information
	m_ID = fiberID();
}
GRAVLib::Concurrency::Fibers::fiberID GRAVLib::Concurrency::Fibers::fiber::getCurrenterFiberID()
{
	fiberID id;

	// Delete the system fiber
	#if GRAVLib_PLATFORM == GRAVLib_PLATFORM_WINDOWS
	id = fiberID(GetCurrentFiber());
	#endif

	// Reset the fiber identifying information
	return id;
}