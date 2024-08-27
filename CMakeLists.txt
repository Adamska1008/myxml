cmake_minimum_required(VERSION 3.22)

project(myxml
        VERSION 0.1)

enable_testing()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(deps/Catch2)
add_subdirectory(deps/fmt)