file(GLOB TEST_SOURCES "src/*.cpp")

add_executable(run_examples ${TEST_SOURCES})

target_link_libraries(run_examples yadl)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${OUTPUT_DIR})