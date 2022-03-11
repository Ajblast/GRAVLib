cmake_minimum_required(VERSION 3.21)

include(cmake/utils.cmake)

makelib(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/${PROJECT_NAME} SHARED True pch.h ${BuildTests})

target_compile_definitions(${PROJECT_NAME} PUBLIC -D${PROJECT_NAME}_THREAD_LIBRARY=1)