cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER "gcc-13")
# set(CMAKE_CXX_COMPILER "g++-13")

project(CacheSimulatorProject C)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

include_directories(${SOURCE_DIR})

file(GLOB_RECURSE SOURCES ${SOURCE_DIR}/*.c)

add_executable(CacheSimulator.bin ${SOURCES})

if(DEBUG_MODE)
    message("Debug mode enabled")
    target_compile_definitions(CacheSimulator.bin PRIVATE DEBUG_MODE)
else()
    message("Debug mode disabled")
endif()

set_target_properties(CacheSimulator.bin PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
