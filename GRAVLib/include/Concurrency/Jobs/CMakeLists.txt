target_sources(${TargetName} 
	PUBLIC
		Counter.h
		Declaration.h
		JobManager.h
		JobManagerCallbacks.h
		JobManagerOptions.h
		JobPriority.h
		JobSystem.h
)

add_subdirectory(Exceptions)