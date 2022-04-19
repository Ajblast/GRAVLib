#pragma once

#include "GRAVLib/GRAVLibCore.h"
#include "GRAVLib/Concurrency/Threads/ThreadID.h"
#include "GRAVLib/Concurrency/Threads/ThreadTypes.h"
#include "GRAVLib/Concurrency/Threads/TLS.h"


#include <string>
#include <functional>
#include <format>

namespace GRAVLib::Concurrency::Threads
{
	class GRAVLibAPI thread
	{
	public:
		//typedef std::function<unsigned(thread*)> threadCallback;

		using threadCallback = unsigned int(*)(thread*);

	public:
		thread();
		thread(threadCallback callback);
		thread(threadCallback callback, const std::wstring& name);
		thread(threadCallback callback, const std::wstring& name, size_t affinity);
		thread(const thread& other) = delete;
		thread(thread&& other) noexcept;
		thread& operator=(const thread& other) = delete;
		thread& operator=(thread&& other) noexcept;
		~thread();


		// Join this thread with the current thread
		void join();
		// Detach the thread
		void detach();
		// Get the handle and id from the currently running thread
		void initializeFromCurrentThread();
		// Close the thread. WARNING: can lead to undefined behavior
		void close();


		#pragma region Getters
		// Get the thread local storage
		tls* getTLS();

		// Set the thread's affinity
		void setAffinity(size_t affinity);
		// Get the thread's affinity
		size_t getAffinity();

		// Get thread name
		std::wstring getName() const;
		// Set the thread's name
		void setName(const std::wstring& name);

		// Get thread identifiying information
		threadID getID() const;
		// Get the thread ID
		threadID_t getThreadID() const;
		// Get the thread native handle
		threadHandle_t getThreadHandle() const;

		// Is the current thread valid
		bool valid() const;
		#pragma endregion

		// Cause the thread to sleep for so many milliseconds
		static void sleepFor(uint32 ms);

		// Get the current thread ID. Handle is not included because a pseudo handle always points to the "current thread" 
		static threadID getCurrentThreadID();
		// Get the number of cores there are
		static uint32 getHardwareConcurrency();

		friend std::formatter<GRAVLib::Concurrency::Threads::thread>;

	private:
		threadID m_ID;				// Thread's ID
		tls m_ThreadLocalStorage;	// The thread local storage of the thread
	};


	inline tls* thread::getTLS() { return &m_ThreadLocalStorage; }
	inline threadID thread::getID() const { return m_ID; }
	inline threadID_t thread::getThreadID() const { return getID().m_ThreadID; }
	inline threadHandle_t thread::getThreadHandle() const { return getID().m_Handle; }

	inline bool thread::valid() const { return getThreadID() != GRAVLib_INVALID_THREAD_ID; }
}

template<>
struct std::formatter<GRAVLib::Concurrency::Threads::thread> : std::formatter<std::string_view>
{
	template<typename FormatContext>
	auto format(const GRAVLib::Concurrency::Threads::thread& thread, FormatContext& ctx)
	{
		return format_to(ctx.out(), "[ID: {} | TLS: {}]", thread.m_ID, thread.m_ThreadLocalStorage);
	}
};
