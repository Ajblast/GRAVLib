cmake_minimum_required(VERSION 3.21)

include(../cmake/utils.cmake)

maketest(${TargetName}Tests ${DynamicRuntime} ${TargetName})

#target_sources(${TestName} 
#	PUBLIC
#		"Events/eventTests.cpp"
#		lockTests.cpp
#		randomTests.cpp
#)

add_subdirectory(Containers)
add_subdirectory(Events)
add_subdirectory(IO)
add_subdirectory(Locks)
add_subdirectory(Memory)
add_subdirectory(Random)
add_subdirectory(Time)
add_subdirectory(Utils)