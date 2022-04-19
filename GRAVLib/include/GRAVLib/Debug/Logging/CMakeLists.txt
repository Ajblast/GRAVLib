target_sources(${TargetName} 
	PUBLIC
		Logging.h
)

add_subdirectory(Loggers)
add_subdirectory(Sinks)