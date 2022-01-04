cmake_minimum_required(VERSION 3.21)

include(../cmake/utils.cmake)

maketest(${TargetName}Tests ${DynamicRuntime} ${TargetName})

target_sources(${TestName} 
	PUBLIC
		eventTests.cpp
		lockTests.cpp
		randomTests.cpp
)

add_subdirectory(Containers)