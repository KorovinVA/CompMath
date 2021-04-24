cmake_minimum_required(VERSION 3.10)

project(FM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE "Release")

include_directories(${CMAKE_SOURCE_DIR}/../benchmark/include)

add_executable(FM src/main.cpp
                  src/hypergraph.h
                  src/hypergraph.cpp
                  src/gaincontainer.h
                  src/gaincontainer.cpp
                  src/algo.h
                  src/algo.cpp
                  src/out.h)

if (WIN32)
    target_link_libraries(FM shlwapi.lib)
endif()

target_link_libraries(FM ${CMAKE_SOURCE_DIR}/../benchmark/build/src/Release/benchmark_main.lib)
target_link_libraries(FM ${CMAKE_SOURCE_DIR}/../benchmark/build/src/Release/benchmark.lib)

if (WIN32)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT FM)
endif()
