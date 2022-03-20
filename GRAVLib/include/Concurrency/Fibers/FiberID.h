#pragma once

#include "GRAVLibCore.h"
#include "FiberTypes.h"
#include <compare>
#include <format>

namespace GRAVLib::Concurrency::Fibers
{
	struct GRAVLibAPI fiberID
	{
	public:
		fiberID();
		fiberID(fiberHandle_t handle);
	public:
		fiberHandle_t m_Handle;	// Native fiber handle
	};

	inline bool operator==(const fiberID& lhs, const fiberID& rhs) noexcept
	{
		return lhs.m_Handle == rhs.m_Handle;
	}
	inline std::strong_ordering operator<=>(const fiberID& lhs, const fiberID& rhs) noexcept
	{
		return lhs.m_Handle <=> rhs.m_Handle;
	}
}

template<>
struct std::formatter<GRAVLib::Concurrency::Fibers::fiberID> : std::formatter<std::string_view>
{
	template<typename FormatContext>
	auto format(const GRAVLib::Concurrency::Fibers::fiberID& id, FormatContext& ctx)
	{
		return format_to(ctx.out(), "[Handle: {}]", id.m_Handle);
	}
};
