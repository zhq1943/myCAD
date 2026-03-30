cmake_minimum_required(VERSION 3.10)
project(myCAD)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    "extern/glfw/include"
    "extern/glad/include"
    "extern/glm"
    "extern/imgui"
    "extern/imgui/backends"
)

link_directories("extern/glfw/lib")

add_executable(myCAD main.cpp extern/glad/src/glad.c extern/imgui/imgui.cpp extern/imgui/imgui_draw.cpp extern/imgui/imgui_tables.cpp extern/imgui/imgui_widgets.cpp extern/imgui/backends/imgui_impl_glfw.cpp extern/imgui/backends/imgui_impl_opengl3.cpp)

target_link_libraries(myCAD glfw3.lib opengl32.lib)