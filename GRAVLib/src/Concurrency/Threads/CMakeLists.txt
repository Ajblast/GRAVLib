target_sources(${TargetName} 
	PUBLIC
		Thread.cpp
		ThreadID.cpp
		ThreadTypes.cpp
		TLS.cpp
)

add_subdirectory(Exceptions)