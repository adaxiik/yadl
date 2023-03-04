file(GLOB YADL_SOURCES "src/*.cpp")

file(GLOB YADL_HEADERS "include/public/yadl/*.hpp")
file(GLOB YADL_PRIVATE_HEADERS "include/private/*.hpp")

set(YADL_INCLUDE_PUBLIC "include/public")
set(YADL_INCLUDE_PRIVATE "include/private")


include_directories("${YADL_INCLUDE_PUBLIC}/yadl")
include_directories("${YADL_INCLUDE_PRIVATE}")

add_library(yadl ${YADL_SOURCES})
target_include_directories(yadl PUBLIC "${YADL_INCLUDE_PUBLIC}")