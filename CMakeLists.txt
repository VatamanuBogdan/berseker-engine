cmake_minimum_required(VERSION 3.1)
project(Berseker)

set(CMAKE_CXX_STANDARD 17)

# Set Berseker parameters
set(BERSEKER_ROOT_DIR ./)
set(BERSEKER_SOURCE_DIR src/)
set(BERSEKER_DEPENDENCIES_DIR deps/)
set(BERSEKER_BINARY_NAME BersekerBin)

set(
    BERSEKER_SOURCES
    ${BERSEKER_SOURCE_DIR}/main.cpp
        src/Rendering/GraphicsAPI/VertexBuffer.hpp
        src/Rendering/GraphicsAPI/VertexArray.hpp
        src/Rendering/GraphicsAPI/IndexBuffer.hpp
        src/Rendering/GraphicsAPI/ShaderProgram.cpp
        src/Rendering/GraphicsAPI/ShaderProgram.h
        src/Rendering/GraphicsAPI/Texture2D.hpp
        src/Rendering/GraphicsAPI/OpenGL.hpp
        src/Rendering/Model.cpp
        src/Rendering/Renderer.cpp
        src/Rendering/Renderer.h
        src/Rendering/Mesh.h
        src/Window/Window.h
        src/Window/GLFW/GLFWWindow.cpp
        src/Window/GLFW/GLFWWindow.h
        src/Input/InputKeys.h
        src/Input/InputManager.h
        src/Application.cpp
        src/Application.h
        src/Scene.h
        src/TestScene/MainScene.cpp
        src/TestScene/MainScene.h
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

add_executable(${BERSEKER_BINARY_NAME} ${BERSEKER_SOURCES})

target_link_libraries(
    ${BERSEKER_BINARY_NAME}
    glfw
    glad
    glm
    spdlog
    stb
    assimp
)

include_directories(${BERSEKER_SOURCE_DIR}/)