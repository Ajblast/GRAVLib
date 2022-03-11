#pragma once

#include "Utils/MacroDefinitions.h"
#include <atomic>

#define GRAVLib_ATOMIC_SWAP(A, B) \
	auto GRAVLib_ANONYMOUS_VARIABLE(temp) = B.exchange(A.load(std::memory_order_acquire)); \
	A.store(GRAVLib_ANONYMOUS_VARIABLE(temp), std::memory_order_release)