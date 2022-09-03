cmake_minimum_required(VERSION 3.1)

set(BERSEKER_TESTING_BINARY Testing)
set(BERSEKER_TESTING_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(BERSEKER_TESTING_SOURCE_DIR ${BERSEKER_TESTING_ROOT_DIR}/src)

set(
    BERSEKER_TESTING_SOURCES
    ${BERSEKER_TESTING_SOURCE_DIR}/Scenes/TestingScene.h
    ${BERSEKER_TESTING_SOURCE_DIR}/Scenes/TestingScene.cpp
    ${BERSEKER_TESTING_SOURCE_DIR}/Scenes/BVHScene.cpp
    ${BERSEKER_TESTING_SOURCE_DIR}/Scenes/BVHScene.h
    src/Scenes/Random.h
    src/Scenes/Timer.h
    src/Scenes/CollisionScene.cpp
    src/Scenes/CollisionScene.h
    src/Scenes/GJKScene.cpp src/Scenes/GJKScene.h)

add_library(
    ${BERSEKER_TESTING_BINARY}
    ${BERSEKER_TESTING_SOURCES}
)

target_link_libraries(
    ${BERSEKER_TESTING_BINARY}
    Engine
)

target_include_directories(
    ${BERSEKER_TESTING_BINARY}
    PUBLIC ${BERSEKER_TESTING_SOURCE_DIR}/
)