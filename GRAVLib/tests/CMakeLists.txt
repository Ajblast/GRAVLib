cmake_minimum_required(VERSION 3.21)

include(../cmake/utils.cmake)

set(SourceFiles
	eventTests.cpp
	lockTests.cpp
	randomTests.cpp
)
maketest(${TargetName}Tests ${DynamicRuntime} "${SourceFiles}" ${TargetName})
