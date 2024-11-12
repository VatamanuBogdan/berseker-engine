cmake_minimum_required(VERSION 3.4)

# Parameters Setup

set(EXAMPLE_LIBRARY_NAME Example)
set(EXAMPLE_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Sources Setup

set(
    EXAMPLE_SOURCES
    src/Scenes/TestingScene.h
    src/Scenes/TestingScene.cpp
    src/Scenes/BVHScene.cpp
    src/Scenes/BVHScene.h
    src/Utils/Random.h
    src/Utils/Timer.h
    src/Scenes/CollisionScene.cpp
    src/Scenes/CollisionScene.h
    src/Scenes/GJKScene.cpp src/Scenes/GJKScene.h)

# Library Setup

add_library(
    ${EXAMPLE_LIBRARY_NAME}
    ${EXAMPLE_SOURCES}
)

target_link_libraries(
    ${EXAMPLE_LIBRARY_NAME}
    Engine
)

target_include_directories(
    ${EXAMPLE_LIBRARY_NAME}
    PUBLIC ${EXAMPLE_SOURCES_DIR}
)