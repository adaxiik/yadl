file(GLOB TEST_SOURCES "src/*.cpp")

add_executable(examples ${TEST_SOURCES})

target_link_libraries(examples yadl)