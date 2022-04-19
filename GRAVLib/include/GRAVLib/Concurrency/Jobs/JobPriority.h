#pragma once

#include "GRAVLib/GRAVLibCore.h"
#include <string.h>
#include <format>

#define GRAVLib_JOB_PRIORITY_LOW		0
#define GRAVLib_JOB_PRIORITY_NORMAL		1
#define GRAVLib_JOB_PRIORITY_HIGH		2
#define GRAVLib_JOB_PRIORITY_CRITICAL	3

#define GRAVLib_JOB_PRIORITY_STRING_LOW			"LOW"
#define GRAVLib_JOB_PRIORITY_STRING_NORMAL		"NORMAL"
#define GRAVLib_JOB_PRIORITY_STRING_HIGH		"HIGH"
#define GRAVLib_JOB_PRIORITY_STRING_CRITICAL	"CIRTICAL"


namespace GRAVLib::Concurrency::Jobs
{
	enum class jobPriority
	{
		LOW			= GRAVLib_JOB_PRIORITY_LOW,
		NORMAL		= GRAVLib_JOB_PRIORITY_NORMAL,
		HIGH		= GRAVLib_JOB_PRIORITY_HIGH,
		CRITICAL	= GRAVLib_JOB_PRIORITY_CRITICAL
	};
}

template<>
struct std::formatter<GRAVLib::Concurrency::Jobs::jobPriority> : std::formatter<std::string_view>
{
	template<typename FormatContext>
	auto format(const GRAVLib::Concurrency::Jobs::jobPriority& priority, FormatContext& ctx)
	{
		std::string priorityName;
		switch (priority)
		{
		case GRAVLib::Concurrency::Jobs::jobPriority::LOW:
			priorityName = GRAVLib_JOB_PRIORITY_STRING_LOW;
			break;
		case GRAVLib::Concurrency::Jobs::jobPriority::NORMAL:
			priorityName = GRAVLib_JOB_PRIORITY_STRING_NORMAL;
			break;
		case GRAVLib::Concurrency::Jobs::jobPriority::HIGH:
			priorityName = GRAVLib_JOB_PRIORITY_STRING_HIGH;
			break;
		case GRAVLib::Concurrency::Jobs::jobPriority::CRITICAL:
			priorityName = GRAVLib_JOB_PRIORITY_STRING_CRITICAL;
			break;
		default:
			priorityName = "UNKNOWN";
			break;
		}

		return format_to(ctx.out(), "[{}]", priorityName);
	}
};
