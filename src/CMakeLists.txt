file(GLOB sources "*.cpp")

add_library(myxml ${sources})

target_include_directories(myxml PUBLIC ".")

target_link_libraries(myxml fmt::fmt)