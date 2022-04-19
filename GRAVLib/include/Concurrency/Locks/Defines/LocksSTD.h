#pragma once

#include "Utils/MacroDefinitions.h"
#include <mutex>

#define GRAVLib_LOCK_TYPE std::recursive_mutex
#define GRAVLib_MUTEX_IMPL(name) mutable GRAVLib_LOCK_TYPE name

#define GRAVLib_LOCK_MUTEX_IMPL(name) std::unique_lock<GRAVLib_LOCK_TYPE> GRAVLib_ANONYMOUS_VARIABLE(GRAVLibLock) (name)
#define GRAVLib_LOCK_NAMED_MUTEX_IMPL(mutexName, lockName) std::unique_lock<GRAVLib_LOCK_TYPE> lockName(mutexName)
#define GRAVLib_LOCK_LOCKS_IMPL(...) std::scoped_lock GRAVLib_ANONYMOUS_VARIABLE(GRAVLibLock) (__VA_ARGS__)

#define GRAVLib_AUTO_MUTEX_IMPL mutable GRAVLib_LOCK_TYPE GRAVLib_AUTO_MUTEX_NAME
#define GRAVLib_LOCK_AUTO_MUTEX_IMPL std::unique_lock<GRAVLib_LOCK_TYPE> GRAVLibAutoMutexLock(GRAVLib_AUTO_MUTEX_NAME)