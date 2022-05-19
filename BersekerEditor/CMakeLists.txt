cmake_minimum_required(VERSION 3.1)

set(BERSEKER_EDITOR_BINARY BersekerEditor)
set(BERSEKER_EDITOR_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(BERSEKER_EDITOR_SOURCE_DIR ${BERSEKER_EDITOR_ROOT_DIR}/src)

set(
    BERSEKER_EDITOR_SOURCES
    ${BERSEKER_EDITOR_SOURCE_DIR}/main.cpp
)

add_executable(
    ${BERSEKER_EDITOR_BINARY}
    ${BERSEKER_EDITOR_SOURCES}
)

target_link_libraries(
    ${BERSEKER_EDITOR_BINARY}
    BersekerTesting
    BersekerEngine
)