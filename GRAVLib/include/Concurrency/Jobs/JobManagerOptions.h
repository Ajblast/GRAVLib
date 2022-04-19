#pragma once

#include "GRAVLibCore.h"
#include "Concurrency/Threads/ThreadTypes.h"
#include "Concurrency/Fibers/FiberTypes.h"
#include "JobManagerCallbacks.h"
#include <format>

namespace GRAVLib::Concurrency::Jobs
{
	enum class queueBehavior
	{
		SPIN,	// Spin lock while looking for jobs
		YIELD,	// Spin lock, but yield after every search
		SLEEP	// Sleep until woken up
	};

	struct GRAVLibAPI jobManagerOptions
	{
	public:
		jobManagerOptions();
		jobManagerOptions(const jobManagerOptions& other) = default;
		jobManagerOptions(jobManagerOptions&& other) = default;
		jobManagerOptions& operator=(const jobManagerOptions& other) = default;
		jobManagerOptions& operator=(jobManagerOptions&& other) = default;
		~jobManagerOptions() = default;

	public:
		// Threads and Fibers
		Threads::threadIndex_t m_NumThreads;		// Number of worker threads. Default to the number of cores
		Fibers::fiberIndex_t m_NumFibers = 128;		// Number of Fibers. Must be at least the number of threads
		bool m_ThreadAffinity = true;				// Should threads be locked to cores. Only valid if the m_NumThreads == Num of cores

		// Wait fiber size
		Fibers::fiberIndex_t m_MaxWaitingFibersCount = 256;

		queueBehavior m_QueueBehavior = queueBehavior::SPIN;	// Threads spin by default

		jobManagerCallbacks m_Callbacks;	// All of the job manager callbacks
	};
}

template<>
struct std::formatter<GRAVLib::Concurrency::Jobs::queueBehavior> : std::formatter<std::string_view>
{
	template<typename FormatContext>
	auto format(const GRAVLib::Concurrency::Jobs::queueBehavior& behavior, FormatContext& ctx)
	{
		std::string value;
		switch (behavior)
		{
		case GRAVLib::Concurrency::Jobs::queueBehavior::SPIN:
			value = "SPIN";
			break;
		case GRAVLib::Concurrency::Jobs::queueBehavior::YIELD:
			value = "YIELD";
			break;
		case GRAVLib::Concurrency::Jobs::queueBehavior::SLEEP:
			value = "SLEEP";
			break; 
		default:
			value = "UNKNOWN";
			break;
		}
		return format_to(ctx.out(), "[{}]", value);
	}
};

template<>
struct std::formatter<GRAVLib::Concurrency::Jobs::jobManagerOptions> : std::formatter<std::string_view>
{
	template<typename FormatContext>
	auto format(const GRAVLib::Concurrency::Jobs::jobManagerOptions& options, FormatContext& ctx)
	{
		return format_to(ctx.out(), "[ThreadCount: {} | FiberCount: {} | ThreadAffinity: {} | MaxWaitingFibers: {} | QueueBehavior: {}]",
			options.m_NumThreads, options.m_NumFibers, options.m_ThreadAffinity, options.m_MaxWaitingFibersCount, options.m_QueueBehavior);
	}
};