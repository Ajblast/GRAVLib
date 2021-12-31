target_sources(${TargetName} 
	PUBLIC
		GRAVLibCore.h
		Platform.h
		Primitives.h
)

add_subdirectory(Events)
add_subdirectory(Memory)
add_subdirectory(Locks)