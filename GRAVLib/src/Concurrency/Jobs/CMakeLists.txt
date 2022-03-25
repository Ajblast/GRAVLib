target_sources(${TargetName} 
	PUBLIC
		Counter.cpp
		Declaration.cpp
		JobManager.cpp
		JobManagerCallbacks.cpp
		JobManagerOptions.cpp
		JobPriority.cpp
)

add_subdirectory(Exceptions)