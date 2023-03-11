file(GLOB YADL_SOURCES "src/*.cpp")
file(GLOB YADL_HEADERS "include/public/yadl/*.hpp")
file(GLOB YADL_PRIVATE_HEADERS "include/private/*.hpp")

set(YADL_INCLUDE_PUBLIC "include/public")
set(YADL_INCLUDE_PRIVATE "include/private")

find_package(Freetype REQUIRED)
message(STATUS "FREETYPE_LIBRARIES: ${FREETYPE_LIBRARIES}")
message(STATUS "FREETYPE_INCLUDE_DIRS: ${FREETYPE_INCLUDE_DIRS}")

add_library(yadl SHARED ${YADL_SOURCES})

target_include_directories(yadl PRIVATE "${FREETYPE_INCLUDE_DIRS}")
target_include_directories(yadl PRIVATE "${YADL_INCLUDE_PUBLIC}/yadl")
target_include_directories(yadl PRIVATE "${YADL_INCLUDE_PRIVATE}")

target_link_libraries(yadl PRIVATE "${FREETYPE_LIBRARIES}")


target_include_directories(yadl PUBLIC "${YADL_INCLUDE_PUBLIC}")