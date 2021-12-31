cmake_minimum_required(VERSION 3.21)

include(../cmake/utils.cmake)

maketest(${TargetName}Tests ${DynamicRuntime} testFile.cpp)
