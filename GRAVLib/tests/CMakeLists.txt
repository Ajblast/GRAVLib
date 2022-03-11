cmake_minimum_required(VERSION 3.21)

include(../cmake/utils.cmake)

maketest(${TargetName}Tests ${DynamicRuntime} ${TargetName})

target_compile_definitions(${TargetName}Tests PUBLIC -D${TestName}_PATH="${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(Containers)
add_subdirectory(Debug)
add_subdirectory(Events)
add_subdirectory(IO)
add_subdirectory(Memory)
add_subdirectory(Random)
add_subdirectory(Time)
add_subdirectory(Utils)