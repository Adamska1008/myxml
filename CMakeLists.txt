cmake_minimum_required(VERSION 3.22)

project(myxml
        VERSION 0.1)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(deps/Catch2)