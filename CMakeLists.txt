cmake_minimum_required(VERSION 3.1)
project(Berseker)

set(CMAKE_CXX_STANDARD 17)

# Set Berseker parameters
set(BERSEKER_SOURCE_DIR src/)
set(BERSEKER_DEPENDENCIES_DIR deps/)
set(BERSEKER_BINARY_NAME BersekerBin)

set(
    BERSEKER_SOURCES
    ${BERSEKER_SOURCE_DIR}/main.cpp
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

add_executable(${BERSEKER_BINARY_NAME} ${BERSEKER_SOURCES})

target_link_libraries(
    ${BERSEKER_BINARY_NAME}
    glfw
    glad
    glm
    spdlog
)