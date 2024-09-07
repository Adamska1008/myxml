file(GLOB sources "*.cpp")

add_library(myxml ${sources})

target_include_directories(myxml PUBLIC ${PROJECT_SOURCE_DIR}/include/)

target_link_libraries(myxml PRIVATE fmt::fmt)

if (ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    message(STATUS "Adding cover flags ${COVERAGE_FLAGS}.")
    target_compile_options(myxml PRIVATE ${COVERAGE_FLAGS})
    target_link_options(myxml PRIVATE ${COVERAGE_FLAGS})
endif()