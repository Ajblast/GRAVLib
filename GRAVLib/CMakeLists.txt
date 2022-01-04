cmake_minimum_required(VERSION 3.21)

include(cmake/utils.cmake)

makelib(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/${PROJECT_NAME} SHARED True pch.h ${BuildTests})

#get_target_property(SourceFiles ${PROJECT_NAME} SOURCES)
#message("Source Files: ${SourceFiles}")