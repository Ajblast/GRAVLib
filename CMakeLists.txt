cmake_minimum_required(VERSION 3.21)

#Default tests
if(NOT DEFINED BuildTests)
	set(BuildTests True)
	enable_testing()
endif()

#Set default build types
if(NOT DEFINED BuildTypes)
	set(BuildTypes
		RELEASE
		RELWITHDEBINFO
		MINSIZEREL
		DEBUG
	)
endif()

#Enable debug by default if the varaible isn't set
if(NOT DEFINED DebugInfo)
	set(DebugInfo True)
endif()

set(ProjectLanguage CXX)
project(GRAVLib VERSION 1.0 LANGUAGES ${ProjectLanguage})

#Set C++ languages
if (${ProjectLanguage} STREQUAL CXX)
	set(CMAKE_C_STANDARD 17)
	set(CMAKE_C_STANDARD_REQUIRED True)
	set(CMAKE_CXX_STANDARD 23)
	set(CMAKE_CXX_STANDARD_REQUIRED True)
endif()

#Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})


#Include Google Test
set(BUILD_GMOCK OFF)
set(gtest_force_shared_crt ON)
add_subdirectory(GoogleTest)

#Include GRAVLib
add_subdirectory(GRAVLib)
