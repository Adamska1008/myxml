include(CTest)

add_executable(element_test element_test.cpp)
add_executable(parser_test parser_test.cpp)

target_link_libraries(element_test Catch2::Catch2WithMain myxml)
target_link_libraries(parser_test Catch2::Catch2WithMain myxml)

target_compile_features(element_test PRIVATE cxx_std_17)
target_compile_features(parser_test PRIVATE cxx_std_17)

add_test(NAME element_test COMMAND element_test)
add_test(NAME parser_test COMMAND parser_test)