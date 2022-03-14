#pragma once

#include "GRAVLibCore.h"
#include "ThreadTypes.h"
#include <compare>
#include <format>

namespace GRAVLib::Concurrency::Threads
{
	struct GRAVLibAPI threadID
	{
	public:
		threadID();
		threadID(threadID_t threadID, threadHandle_t handle, threadIndex_t index);
		threadID(const threadID& other);
		threadID(threadID&& other) noexcept;
		threadID& operator=(const threadID& other);
		threadID& operator=(threadID&& other) noexcept;
	public:
		threadID_t m_ThreadID;		// Thread ID
		threadHandle_t m_Handle;	// Native thread handle
		threadIndex_t m_Index;		// Thread index
	};

	inline bool operator==(const threadID& lhs, const threadID& rhs) noexcept
	{
		return lhs.m_ThreadID == rhs.m_ThreadID && lhs.m_Handle == rhs.m_Handle && lhs.m_Index == rhs.m_Index;
	}
	inline std::strong_ordering operator<=>(const threadID& lhs, const threadID& rhs) noexcept
	{
		if (auto cmp = lhs.m_Handle <=> rhs.m_Handle; cmp != 0) return cmp;
		if (auto cmp = lhs.m_Index <=> rhs.m_Index; cmp != 0) return cmp;

		return lhs.m_ThreadID <=> rhs.m_ThreadID;
	}
}

template<>
struct std::formatter<GRAVLib::Concurrency::Threads::threadID> : std::formatter<std::string_view>
{
	template<typename FormatContext>
	auto format(const GRAVLib::Concurrency::Threads::threadID& id, FormatContext& ctx)
	{
		return format_to(ctx.out(), "[ThreadID: {} | Handle: {} | Index: {}]", id.m_ThreadID, id.m_Handle, id.m_Index);
	}
};
