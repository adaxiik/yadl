#imgui
file(GLOB IMGUI_SRC "sdl_examples/sdl_core/3rdparty/imgui/*.cpp")
file(GLOB IMGUI_BACKENDS_SRC "sdl_examples/sdl_core/3rdparty/imgui/backends/*.cpp")
set(IMGUI_SRC ${IMGUI_SRC} ${IMGUI_BACKENDS_SRC})
file(GLOB IMGUI_HEADERS "sdl_examples/sdl_core/3rdparty/imgui/*.h")

add_library(imgui STATIC ${IMGUI_SRC} ${IMGUI_HEADERS})
target_include_directories(imgui PUBLIC "sdl_examples/sdl_core/3rdparty/imgui")


# classic examples
add_executable(classic_examples "classic_examples/examples.cpp")
target_link_libraries(classic_examples yadl m)

# sdl examples
# circle 
add_executable(circle_example "sdl_examples/circle_example.cpp")
target_include_directories(circle_example PUBLIC "sdl_examples/sdl_core/3rdparty/imgui")
target_link_libraries(circle_example yadl m SDL2)
target_link_libraries(circle_example imgui)

# text
add_executable(text_example "sdl_examples/text_example.cpp")
target_include_directories(text_example PUBLIC "sdl_examples/sdl_core/3rdparty/imgui")
target_link_libraries(text_example yadl m SDL2)
target_link_libraries(text_example imgui)

# paint
add_executable(paint_example "sdl_examples/paint_example.cpp")
target_include_directories(paint_example PUBLIC "sdl_examples/sdl_core/3rdparty/imgui")
target_link_libraries(paint_example yadl m SDL2)
target_link_libraries(paint_example imgui)

# gfx
add_executable(gfx_example "sdl_examples/gfx_example.cpp")
target_include_directories(gfx_example PUBLIC "sdl_examples/sdl_core/3rdparty/imgui")
target_link_libraries(gfx_example yadl m SDL2)
target_link_libraries(gfx_example imgui)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${OUTPUT_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/clean_generated_examples.sh DESTINATION ${OUTPUT_DIR})