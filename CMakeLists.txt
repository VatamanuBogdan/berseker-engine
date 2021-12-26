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
        src/Graphics/OpenGL/VertexBuffer.hpp
        src/Graphics/OpenGL/VertexArray.hpp
        src/Graphics/OpenGL/IndexBuffer.hpp
        src/Graphics/OpenGL/ShaderProgram.cpp
        src/Graphics/OpenGL/ShaderProgram.h
        src/Graphics/OpenGL/Texture2D.hpp
        src/Graphics/OpenGL/OpenGL.hpp
        src/Graphics/Model.cpp
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