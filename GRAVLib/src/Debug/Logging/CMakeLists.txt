target_sources(${TargetName} 
	PRIVATE
		Logging.cpp
)

add_subdirectory(Loggers)
add_subdirectory(Sinks)