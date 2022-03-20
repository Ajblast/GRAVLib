target_sources(${TargetName} 
	PUBLIC
		Locks.h
		MultiLock.h
		RecursiveLock.h
		ScopedLock.h
		SpinLock.h
		UnnecessaryLock.h
)

add_subdirectory(Defines)