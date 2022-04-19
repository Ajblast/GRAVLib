#pragma once

#include "GRAVLib/GRAVLibCore.h"

#define GRAVLib_INVALID_FIBER_INDEX UINT16_MAX

namespace GRAVLib::Concurrency::Fibers
{
	typedef uint16 fiberIndex_t;	// A fiber index
	typedef void* fiberHandle_t;	// A handle to a fiber
}