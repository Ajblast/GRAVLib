#pragma once

#include "GRAVLibCore.h"

#define GRAVLib_INVALID_THREAD_ID 0
#define GRAVLib_INVALID_THREAD_INDEX UINT8_MAX

namespace GRAVLib::Concurrency::Threads
{
	typedef uint32 threadID_t;		// A thread ID
	typedef uint8 threadIndex_t;	// A thread index
	typedef void* threadHandle_t;	// A thread handle
}