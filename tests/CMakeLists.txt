include(CTest)



add_executable(element_test element_test.cpp)
add_executable(parser_test parser_test.cpp)
add_executable(exportable_test exportable_test.cpp)
add_executable(document_test document_test.cpp)
add_executable(buffer_test buffer_test.cpp)

target_link_libraries(element_test Catch2::Catch2WithMain myxml)
target_link_libraries(parser_test Catch2::Catch2WithMain myxml)
target_link_libraries(exportable_test Catch2::Catch2WithMain myxml)
target_link_libraries(document_test Catch2::Catch2WithMain myxml)
target_link_libraries(buffer_test Catch2::Catch2WithMain myxml)

target_compile_features(element_test PRIVATE cxx_std_17)
target_compile_features(parser_test PRIVATE cxx_std_17)
target_compile_features(exportable_test PRIVATE cxx_std_17)
target_compile_features(document_test PRIVATE cxx_std_17)
target_compile_features(buffer_test PRIVATE cxx_std_17)

add_test(NAME element_test COMMAND element_test)
add_test(NAME parser_test COMMAND parser_test)
add_test(NAME exportable_test COMMAND exportable_test)
add_test(NAME document_test COMMAND document_test)
add_test(NAME buffer_test COMMAND buffer_test)

set_tests_properties(buffer_test PROPERTIES WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/tests)