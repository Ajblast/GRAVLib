#include "Concurrency/Threads/Thread.h"
#include "Concurrency/Threads/Exceptions/ThreadException.h"
#include "Concurrency/Threads/Exceptions/ThreadCreationException.h"
#include "Concurrency/Threads/Exceptions/ThreadAffinityException.h"
#include "Debug/Logging/Logging.h"

#if GRAVLib_PLATFORM == GRAVLib_PLATFORM_WINDOWS
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <process.h>
#endif


GRAVLib::Concurrency::Threads::thread::thread() 
{}
GRAVLib::Concurrency::Threads::thread::thread(threadCallback callback)
{
	threadHandle_t handle = nullptr;
	unsigned int id = GRAVLib_INVALID_THREAD_ID;

	// Create the thread
	#if GRAVLib_PLATFORM == GRAVLib_PLATFORM_WINDOWS
	handle = reinterpret_cast<void*>(_beginthreadex(nullptr, 0, (_beginthreadex_proc_type)callback, this, 0, (unsigned int*)&id));
	//handle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)callback, this, 0, (DWORD*)&id);
	#endif

	// Throw an exception if a thread was not able to be created
	if (handle == NULL)
		throw GRAVLib::Concurrency::Threads::Exceptions::threadCreationException("Unable to spawn thread.");

	// Set the thread handle and ID
	m_ID = threadID(id, handle);
}
GRAVLib::Concurrency::Threads::thread::thread(threadCallback callback, const std::wstring& name, size_t affinity) : thread(callback)
{
	setName(name);
	setAffinity(affinity);
}
GRAVLib::Concurrency::Threads::thread::thread(thread&& other) noexcept
{
	*this = std::move(other);
}
GRAVLib::Concurrency::Threads::thread& GRAVLib::Concurrency::Threads::thread::operator=(thread&& other) noexcept
{
	if (this != &other)
	{
		GRAV_ASSERT_FALSE(valid()); // The current thread must not be valid to me set.

		// If the current thread is valid, we are trying to overwrite a running thread, which is really bad.
		if (valid())
			terminate();

		// Because threads shouldn't share information, we just flat out steal the other thread's stuff and set theirs to null
		m_ID = std::exchange(other.m_ID, {});
		m_ThreadLocalStorage = std::exchange(other.m_ThreadLocalStorage, {});
	}

	return *this;
}
GRAVLib::Concurrency::Threads::thread::~thread()
{
	GRAV_ASSERT_FALSE(valid()); // The current thread must not be valid to me set.

	// If the current thread is valid, we are trying delete the object representing the object representing the native handle, which is really bad.
	if (valid())
	{
		GRAVLib_LOG_LINE_CRITICAL("Attempting to call destructor on still valid thread [{}].", *this);
		terminate();
	}
}


void GRAVLib::Concurrency::Threads::thread::join()
{
	GRAV_ASSERT_TRUE(valid()); // Must be valid to join this thread

	// Join the thread handle
	#if GRAVLib_PLATFORM == GRAVLib_PLATFORM_WINDOWS
	WaitForSingleObject(getThreadHandle(), INFINITE);
	#endif

	// Close the thread's handle
	close();
}
void GRAVLib::Concurrency::Threads::thread::detach()
{
	GRAV_ASSERT_TRUE(valid()); // Must be valid to join this thread

	// Close the handle to detach it from this object
	close();
}
void GRAVLib::Concurrency::Threads::thread::initializeFromCurrentThread()
{
	GRAV_ASSERT_FALSE(valid()); // Thread must not be valid to initialize from the current thread

	// Get the current thread information
	#if GRAVLib_PLATFORM == GRAVLib_PLATFORM_WINDOWS
	m_ID.m_Handle = GetCurrentThread();
	m_ID.m_ThreadID = GetCurrentThreadId();
	#endif
}
void GRAVLib::Concurrency::Threads::thread::close()
{
	GRAV_ASSERT_TRUE(valid());

	// Close the handle
	#if GRAVLib_PLATFORM == GRAVLib_PLATFORM_WINDOWS
	CloseHandle(getThreadHandle());
	#endif

	// Clear out the identifying information
	m_ID = {};
}

void GRAVLib::Concurrency::Threads::thread::setAffinity(size_t affinity)
{
	GRAV_ASSERT_TRUE(valid()); // Must be valid to set thread affinity

	#if GRAVLib_PLATFORM == GRAVLib_PLATFORM_WINDOWS
	// Mask the CPU and set the affinity
	DWORD_PTR mask = 1ull << affinity;
	SetThreadAffinityMask(getThreadHandle(), mask);
	#endif
}
size_t GRAVLib::Concurrency::Threads::thread::getAffinity()
{
	GRAV_ASSERT_TRUE(valid()); // Must be valid to get thread affinity

	size_t affinity = 0;

	#if GRAVLib_PLATFORM == GRAVLib_PLATFORM_WINDOWS
	size_t mask = 1;
	while (mask)
	{
		// Try each CPU to set the mask to get the original mask
		// This is done like this because the process itself could disallow a certain CPU and we need to succeed to get the original mask
		size_t old = SetThreadAffinityMask(getThreadHandle(), mask);
		if (old)
		{
			SetThreadAffinityMask(getThreadHandle(), old);
			affinity = old;
			break;
		}
		else
			if (GetLastError() != ERROR_INVALID_PARAMETER)
				throw Exceptions::threadAffinityException("Unable to retrieve original affinity mask");

		// Go to the next CPU
		mask <<= 1;
	}
	#endif

	// Affinities are set 0 based, so make this zero based
	return affinity - 1;
}

void GRAVLib::Concurrency::Threads::thread::setName(const std::wstring& name)
{
	GRAV_ASSERT_TRUE(valid()); // Must be valid to set thread name

	#if GRAVLib_PLATFORM == GRAVLib_PLATFORM_WINDOWS
	PCWSTR threadName = (PCWSTR)name.c_str();
	SetThreadDescription(getThreadHandle(), threadName);
	#endif
}
std::wstring GRAVLib::Concurrency::Threads::thread::getName() const
{
	GRAV_ASSERT_TRUE(valid()); // Must be valid to get thread name

	std::wstring threadName;

	#if GRAVLib_PLATFORM == GRAVLib_PLATFORM_WINDOWS
	PWSTR data;
	HRESULT hr = GetThreadDescription(getThreadHandle(), &data);

	if (SUCCEEDED(hr))
	{
		threadName = std::wstring(data);
		LocalFree(data);
	}
	#endif

	return threadName;
}

void GRAVLib::Concurrency::Threads::thread::sleepFor(uint32 ms)
{
	#if GRAVLib_PLATFORM == GRAVLib_PLATFORM_WINDOWS
	Sleep(ms);
	#elif GRAVLib_PLATFORM == GRAVLib_PLATFORM_LINUX
	sleep(ms);
	#endif
}
GRAVLib::Concurrency::Threads::threadID GRAVLib::Concurrency::Threads::thread::getCurrentThreadID()
{
	threadID_t id;

	// Close the handle
	#if GRAVLib_PLATFORM == GRAVLib_PLATFORM_WINDOWS
	id = GetCurrentThreadId();
	#endif

	// Clear out the identifying information
	return threadID(id, nullptr);
}
