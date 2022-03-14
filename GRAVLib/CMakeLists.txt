cmake_minimum_required(VERSION 3.21)

include(cmake/utils.cmake)

makelib(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/${PROJECT_NAME} SHARED True pch.h ${BuildTests})

option(enableProfiling "Enable Profiler" OFF)
cmake_dependent_option(enableConsoleProfiling "Enable Console Logging" OFF enableProfiling OFF)


target_compile_definitions(${PROJECT_NAME} PUBLIC -D${PROJECT_NAME}_THREAD_LIBRARY=1)

if(enableProfiling)
	target_compile_definitions(${PROJECT_NAME} PUBLIC -D${PROJECT_NAME}_PROFILE)
endif()

if(enableProfiling)
	target_compile_definitions(${PROJECT_NAME} PUBLIC -D${PROJECT_NAME}_PROFILE_CONSOLE)
endif()