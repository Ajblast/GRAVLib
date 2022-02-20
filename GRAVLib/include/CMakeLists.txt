target_sources(${TargetName} 
	PUBLIC
		GRAVLibCore.h
		Platform.h
		Primitives.h
)

add_subdirectory(Containers)
add_subdirectory(Events)
add_subdirectory(Locks)
add_subdirectory(Memory)
add_subdirectory(Random)
add_subdirectory(Time)
add_subdirectory(Utils)