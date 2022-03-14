#pragma once

#include "GRAVLibCore.h"
#include <atomic>
#include <format>

#define GRAVLib_INVALID_FIBER_INDEX UINT16_MAX

namespace GRAVLib::Concurrency::Fibers
{
	typedef uint16 fiberIndex_t;	// A fiber index
	typedef void* fiberHandle_t;	// A handle to a fiber

	struct fiberBundle
	{
		fiberIndex_t m_Index;				// The fiber's index
		std::atomic_bool m_FiberIsSwitched;	// Has the fiber been switch out of and cleaned up
	};
}

template<>
struct std::formatter<GRAVLib::Concurrency::Fibers::fiberBundle> : std::formatter<std::string_view>
{
	template<typename FormatContext>
	auto format(const GRAVLib::Concurrency::Fibers::fiberBundle& bundle, FormatContext& ctx)
	{
		return format_to(ctx.out(), "[Index: {} | IsFiberSwitched: {}]", bundle.m_Index, bundle.m_FiberIsSwitched);
	}
};
