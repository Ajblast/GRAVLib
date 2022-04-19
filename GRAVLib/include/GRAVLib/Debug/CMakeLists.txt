target_sources(${TargetName} 
	PUBLIC
		Assertions.h
)

add_subdirectory(Instrumentation)
add_subdirectory(Logging)