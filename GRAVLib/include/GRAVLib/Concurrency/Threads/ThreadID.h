#pragma once

#include "GRAVLib/GRAVLibCore.h"
#include "GRAVLib/Concurrency/Threads/ThreadTypes.h"
#include <compare>
#include <format>
#include <functional>

namespace GRAVLib::Concurrency::Threads
{
	struct GRAVLibAPI threadID
	{
	public:
		threadID();
		threadID(threadID_t threadID, threadHandle_t handle);
	public:
		threadID_t m_ThreadID;		// Thread ID
		threadHandle_t m_Handle;	// Native thread handle
	};

	inline bool operator==(const threadID& lhs, const threadID& rhs) noexcept
	{
		// ThreadIDs are only checked by the actual internal id instead without handles because pseudo handles can be given
		return lhs.m_ThreadID == rhs.m_ThreadID;
	}
	inline std::strong_ordering operator<=>(const threadID& lhs, const threadID& rhs) noexcept
	{
		return lhs.m_ThreadID <=> rhs.m_ThreadID;
	}
}

template<>
struct std::formatter<GRAVLib::Concurrency::Threads::threadID> : std::formatter<std::string_view>
{
	template<typename FormatContext>
	auto format(const GRAVLib::Concurrency::Threads::threadID& id, FormatContext& ctx)
	{
		return format_to(ctx.out(), "[ThreadID: {} | Handle: {}]", id.m_ThreadID, id.m_Handle);
	}
};

template<>
struct std::hash<GRAVLib::Concurrency::Threads::threadID>
{
	size_t operator()(const GRAVLib::Concurrency::Threads::threadID& id) const
	{
		// Hash doesn't include native handle
		return std::hash<GRAVLib::Concurrency::Threads::threadID_t>()(id.m_ThreadID);
	}
};
