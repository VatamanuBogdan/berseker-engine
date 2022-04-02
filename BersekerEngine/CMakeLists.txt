cmake_minimum_required(VERSION 3.1)

# Set Berseker parameters
set(BERSEKER_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(BERSEKER_SOURCE_DIR ${BERSEKER_ROOT_DIR}/src/)
set(BERSEKER_DEPENDENCIES_DIR ${BERSEKER_ROOT_DIR}/deps/)
set(BERSEKER_BINARY_NAME BersekerEngine)

set(
    BERSEKER_SOURCES
    ${BERSEKER_SOURCE_DIR}/Rendering/GraphicsAPI/VertexBuffer.hpp
    ${BERSEKER_SOURCE_DIR}/Rendering/GraphicsAPI/VertexArray.hpp
    ${BERSEKER_SOURCE_DIR}/Rendering/GraphicsAPI/IndexBuffer.hpp
    ${BERSEKER_SOURCE_DIR}/Rendering/GraphicsAPI/ShaderProgram.cpp
    ${BERSEKER_SOURCE_DIR}/Rendering/GraphicsAPI/ShaderProgram.h
    ${BERSEKER_SOURCE_DIR}/Rendering/GraphicsAPI/Texture2D.hpp
    ${BERSEKER_SOURCE_DIR}/Rendering/GraphicsAPI/OpenGL.hpp
    ${BERSEKER_SOURCE_DIR}/Rendering/Model.cpp
    ${BERSEKER_SOURCE_DIR}/Rendering/Renderer.cpp
    ${BERSEKER_SOURCE_DIR}/Rendering/Renderer.h
    ${BERSEKER_SOURCE_DIR}/Rendering/Mesh.h
    ${BERSEKER_SOURCE_DIR}/Window/Window.h
    ${BERSEKER_SOURCE_DIR}/Window/GLFW/GLFWWindow.cpp
    ${BERSEKER_SOURCE_DIR}/Window/GLFW/GLFWWindow.h
    ${BERSEKER_SOURCE_DIR}/Input/InputKeys.h
    ${BERSEKER_SOURCE_DIR}/Input/InputManager.h
    ${BERSEKER_SOURCE_DIR}/Application.cpp
    ${BERSEKER_SOURCE_DIR}/Application.h
    ${BERSEKER_SOURCE_DIR}/Scene.h
    ${BERSEKER_SOURCE_DIR}/Rendering/Primitives/Renderer.cpp
    ${BERSEKER_SOURCE_DIR}/Rendering/Primitives/Renderer.h
    ${BERSEKER_SOURCE_DIR}/Rendering/Primitives/Primitives.h
    ${BERSEKER_SOURCE_DIR}/Rendering/Utils.h
    ${BERSEKER_SOURCE_DIR}/Rendering/Utils.cpp
)

# Set glfw library
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${BERSEKER_DEPENDENCIES_DIR}/glfw)

# Set glad library
add_subdirectory(${BERSEKER_DEPENDENCIES_DIR}/glad)

# Set glm library
add_subdirectory(${BERSEKER_DEPENDENCIES_DIR}/glm)

# Set spdlog library
add_subdirectory(${BERSEKER_DEPENDENCIES_DIR}/spdlog)

# Set assimp library
add_subdirectory(${BERSEKER_DEPENDENCIES_DIR}/assimp)

# Set stb library
add_subdirectory(${BERSEKER_DEPENDENCIES_DIR}/stb)

add_library(
    ${BERSEKER_BINARY_NAME}
    ${BERSEKER_SOURCES}
)

target_link_libraries(
    ${BERSEKER_BINARY_NAME}
    glfw
    glad
    glm
    spdlog
    stb
    assimp
)

target_include_directories(
    ${BERSEKER_BINARY_NAME}
    PUBLIC ${BERSEKER_SOURCE_DIR}/
)