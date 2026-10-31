cmake_minimum_required(VERSION 3.15)
project(OpenGLGLadWindow)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

find_package(glfw3 REQUIRED)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)

add_executable(${PROJECT_NAME}
    src/3d_liquid_sim.cpp
    src/shader.cpp 
    external/glad_generated/glad/src/glad.c

    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    
    # --- ImGui Backends for GLFW + OpenGL3 ---
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/shader.vert
    ${CMAKE_CURRENT_BINARY_DIR}/shader.vert
    COPYONLY
)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/shader.frag
    ${CMAKE_CURRENT_BINARY_DIR}/shader.frag
    COPYONLY
)

target_include_directories(${PROJECT_NAME} PRIVATE
    src 
    external/glad_generated/glad/include

    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL 
    glfw
)
