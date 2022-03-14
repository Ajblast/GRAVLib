#pragma once

#include "GRAVLibCore.h"
#include "ThreadID.h"
#include "ThreadTypes.h"
#include "TLS.h"

#include <string>
#include <functional>
#include <format>

namespace GRAVLib::Concurrency::Threads
{
	class GRAVLibAPI thread
	{
	public:
		typedef std::function<unsigned(thread*)> threadCallback;
		//using threadCallback = void(*)(thread*);

	public:
		thread();
		thread(threadCallback callback);
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
		// Get the thread index
		threadIndex_t getThreadIndex() const;
		// Set the thread index
		void setThreadIndex(threadIndex_t index);

		// Is the current thread valid
		bool valid() const;
		#pragma endregion

		// Cause the thread to sleep for so many milliseconds
		static void sleepFor(uint32 ms);
	private:
		void close();

	private:
		threadID m_ID;				// Thread's ID
		tls m_ThreadLocalStorage;	// The thread local storage of the thread
	};


	inline tls* thread::getTLS() { return &m_ThreadLocalStorage; }
	inline threadID thread::getID() const { return m_ID; }
	inline threadID_t thread::getThreadID() const { return getID().m_ThreadID; }
	inline threadHandle_t thread::getThreadHandle() const { return getID().m_Handle; }
	inline threadIndex_t thread::getThreadIndex() const { return getID().m_Index; }
	inline void thread::setThreadIndex(threadIndex_t index) { m_ID.m_Index = index; }

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

