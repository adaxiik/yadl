cmake_minimum_required(VERSION 3.0.0)
project(YADL_PROJ VERSION 0.1)

set (CMAKE_CXX_STANDARD 17)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra ")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -pedantic -flto -march=native -mtune=native")

add_compile_definitions(YADL_SAFE_PIXEL_ACCESS)
add_compile_definitions(YADL_PERF)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_definitions(YADL_DEBUG)
    message("BUILDING WITH DEBUG MODE")
endif()

set(OUTPUT_DIR "${CMAKE_BINARY_DIR}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

add_subdirectory(yadl)
add_subdirectory(examples)
