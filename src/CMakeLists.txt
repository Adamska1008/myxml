file(GLOB sources "*.cpp")

add_library(myxml ${sources})

target_include_directories(myxml PUBLIC ${PROJECT_SOURCE_DIR}/include/)

target_link_libraries(myxml fmt::fmt)