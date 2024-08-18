cmake_minimum_required(VERSION 3.22)

project(myxml
        VERSION 0.1)

add_subdirectory(src)
add_subdirectory(tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)