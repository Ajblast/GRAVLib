target_sources(${TargetName} 
	PUBLIC
		LibExportImport.h
		GRAVLibCore.h
		Platform.h
		Primitives.h
)

add_subdirectory(Concurrency)
add_subdirectory(Containers)
add_subdirectory(Debug)
add_subdirectory(Events)
add_subdirectory(IO)
add_subdirectory(Memory)
add_subdirectory(Random)
add_subdirectory(Time)
add_subdirectory(Utils)