target_sources(${TargetName} 
	PUBLIC
		Logging.cpp
)

add_subdirectory(Loggers)
add_subdirectory(Sinks)