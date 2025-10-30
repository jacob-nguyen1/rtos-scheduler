cmake_minimum_required(VERSION 3.10)

project(rtosScheduler)

set(CMAKE_CXX_STANDARD 14) 
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(rtosSuite ${SOURCES})

set_target_properties(rtosSuite PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY bin
)