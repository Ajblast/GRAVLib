target_sources(${TargetName} 
	PRIVATE
		Thread.cpp
		ThreadID.cpp
		ThreadTypes.cpp
		TLS.cpp
)

add_subdirectory(Exceptions)