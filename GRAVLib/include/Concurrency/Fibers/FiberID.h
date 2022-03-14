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
		fiberID(fiberHandle_t handle, fiberIndex_t index);
		fiberID(const fiberID& other);
		fiberID(fiberID&& other) noexcept;
		fiberID& operator=(const fiberID& other);
		fiberID& operator=(fiberID&& other) noexcept;
	public:
		fiberHandle_t m_Handle;	// Native fiber handle
		fiberIndex_t m_Index;	// Fiber index
	};

	inline bool operator==(const fiberID& lhs, const fiberID& rhs) noexcept
	{
		return lhs.m_Handle == rhs.m_Handle && lhs.m_Index == rhs.m_Index;
	}
	inline std::strong_ordering operator<=>(const fiberID& lhs, const fiberID& rhs) noexcept
	{
		if (auto cmp = lhs.m_Handle <=> rhs.m_Handle; cmp != 0) return cmp;

		return lhs.m_Index <=> rhs.m_Index;
	}
}

template<>
struct std::formatter<GRAVLib::Concurrency::Fibers::fiberID> : std::formatter<std::string_view>
{
	template<typename FormatContext>
	auto format(const GRAVLib::Concurrency::Fibers::fiberID& id, FormatContext& ctx)
	{
		return format_to(ctx.out(), "[Handle: {} | Index: {}]", id.m_Handle, id.m_Index);
	}
};
