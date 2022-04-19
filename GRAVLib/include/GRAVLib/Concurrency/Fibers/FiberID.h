#pragma once

#include "GRAVLib/GRAVLibCore.h"
#include "GRAVLib/Concurrency/Fibers/FiberTypes.h"
#include <compare>
#include <format>
#include <functional>

namespace GRAVLib::Concurrency::Fibers
{
	struct GRAVLibAPI fiberID
	{
	public:
		fiberID();
		fiberID(fiberHandle_t handle);

		const bool valid() const;
	public:
		fiberHandle_t m_Handle;	// Native fiber handle
	};

	inline const bool fiberID::valid() const { return m_Handle != nullptr; }

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

template<>
struct std::hash<GRAVLib::Concurrency::Fibers::fiberID>
{
	size_t operator()(const GRAVLib::Concurrency::Fibers::fiberID& id) const
	{
		return
			std::hash<GRAVLib::Concurrency::Fibers::fiberHandle_t>()(id.m_Handle);
	}
};

