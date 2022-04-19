#pragma once

#include "Utils/MacroDefinitions.h"
#include "Concurrency/Locks/RecursiveLock.h"
#include "Concurrency/Locks/ScopedLock.h"
#include "Concurrency/Locks/MultiLock.h"

#define GRAVLib_LOCK_TYPE GRAVLib::Concurrency::Locks::recursiveLock
#define GRAVLib_MUTEX_IMPL(name) mutable GRAVLib_LOCK_TYPE name

#define GRAVLib_LOCK_MUTEX_IMPL(name) GRAVLib::Concurrency::Locks::scopedLock<decltype(name)> GRAVLib_ANONYMOUS_VARIABLE(GRAVLibLock) (name)
#define GRAVLib_LOCK_NAMED_MUTEX_IMPL(mutexName, lockName) GRAVLib::Concurrency::Locks::scopedLock<decltype(name)> lockName(mutexName)
#define GRAVLib_LOCK_LOCKS_IMPL(...) GRAVLib::Concurrency::Locks::multiLock GRAVLib_ANONYMOUS_VARIABLE(GRAVLibLock) (__VA_ARGS__)

#define GRAVLib_AUTO_MUTEX_IMPL mutable GRAVLib_LOCK_TYPE GRAVLib_AUTO_MUTEX_NAME
#define GRAVLib_LOCK_AUTO_MUTEX_IMPL GRAVLib::Concurrency::Locks::scopedLock<GRAVLib_LOCK_TYPE> GRAVLibAutoMutexLock(GRAVLib_AUTO_MUTEX_NAME)