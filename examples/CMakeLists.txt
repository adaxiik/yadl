file(GLOB TEST_SOURCES "src/*.cpp")

add_executable(run_examples ${TEST_SOURCES})

target_link_libraries(run_examples yadl m)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${OUTPUT_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/clean_generated_examples.sh DESTINATION ${OUTPUT_DIR})