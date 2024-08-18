include(FetchContent)

FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.7.0
)

FetchContent_MakeAvailable(catch2)

include(CTest)

add_executable(element element.cpp)

target_link_libraries(element Catch2)

target_compile_features(element PRIVATE cxx_std_17)

add_test(NAME basic COMMAND element)