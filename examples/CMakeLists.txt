#imgui
file(GLOB IMGUI_SRC "sdl_examples/sdl_core/3rdparty/imgui/*.cpp")
file(GLOB IMGUI_BACKENDS_SRC "sdl_examples/sdl_core/3rdparty/imgui/backends/*.cpp")
set(IMGUI_SRC ${IMGUI_SRC} ${IMGUI_BACKENDS_SRC})
file(GLOB IMGUI_HEADERS "sdl_examples/sdl_core/3rdparty/imgui/*.h")

add_library(imgui STATIC ${IMGUI_SRC} ${IMGUI_HEADERS})
target_include_directories(imgui PUBLIC "sdl_examples/sdl_core/3rdparty/imgui")

# readme example
add_executable(readme_example "classic_examples/readme_example.cpp")
target_link_libraries(readme_example yadl)

# basic examples
add_executable(basic_examples "classic_examples/basic_examples.cpp")
target_link_libraries(basic_examples yadl m)

# cube example
add_executable(cube_example "classic_examples/cube_example.cpp")
target_link_libraries(cube_example yadl m)

# triangle example
add_executable(triangle_example "classic_examples/triangle_example.cpp")
target_link_libraries(triangle_example yadl m)

# texture mapping example
add_executable(texture_mapping_example "classic_examples/texture_mapping_example.cpp")
target_link_libraries(texture_mapping_example yadl m)

# 3d example
add_executable(3d_example "classic_examples/3d_example.cpp")
target_link_libraries(3d_example yadl m)

# model example
add_executable(model_render_example "classic_examples/model_render_example.cpp")
target_link_libraries(model_render_example yadl m)

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