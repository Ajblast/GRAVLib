#pragma once

#include "GRAVLibCore.h"
#include "ThreadTypes.h"
#include "Concurrency/Fibers/Fiber.h"

#include <vector>
#include <format>

namespace GRAVLib::Concurrency::Threads
{
	enum class fiberDestination : uint8
	{
		NONE, WAITNIG, POOL
	};

	// A struct to hold information that is local to a specific thread
	struct tls
	{
		tls();
		tls(const tls& other) = delete;
		tls(tls&& other) noexcept;
		tls& operator=(const tls& other) = delete;
		tls& operator=(tls&& other) noexcept;
		~tls();

		// TODO: Make special threads for IO
		//bool m_IsIOThread = false;

		// The thread's personal starting fiber that is outside of the fiber pool.
		// This is used to start fiber's into the fiber pool, and allows the fiber pool to return back to the calling thread callback after execution ends
		Fibers::fiberID m_Fiber;


		// Current fiber
		Fibers::fiberID m_CurrentFiber;
		// Previous thread fiber
		//Fibers::fiberID m_PreviousFiberID;
	};
}

template<>
struct std::formatter<GRAVLib::Concurrency::Threads::tls> : std::formatter<std::string_view>
{
	template<typename FormatContext>
	auto format(const GRAVLib::Concurrency::Threads::tls& tls, FormatContext& ctx)
	{
		return format_to(ctx.out(), "[Fiber: {} | CurrentFiberID: {} | PreviousFiberID: {}]", 
			tls.m_Fiber, tls.m_CurrentFiber);
	}
};

template<>
struct std::formatter<GRAVLib::Concurrency::Threads::fiberDestination> : std::formatter<std::string_view>
{
	template<typename FormatContext>
	auto format(const GRAVLib::Concurrency::Threads::fiberDestination& destination, FormatContext& ctx)
	{
		switch (destination)
		{
		case GRAVLib::Concurrency::Threads::fiberDestination::NONE:
			return format_to(ctx.out(), "{}", "NONE");
		case GRAVLib::Concurrency::Threads::fiberDestination::POOL:
			return format_to(ctx.out(), "{}", "POOL");
		case GRAVLib::Concurrency::Threads::fiberDestination::WAITNIG:
			return format_to(ctx.out(), "{}", "WAITING");
		default:
			return format_to(ctx.out(), "{}", "UNKNOWN");
		}
	}
};
