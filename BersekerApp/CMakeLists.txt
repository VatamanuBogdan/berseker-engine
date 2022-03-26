cmake_minimum_required(VERSION 3.1)

set(BERSEKER_APP_BINARY BersekerApp)
set(BERSEKER_APP_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(BERSEKER_APP_SOURCE_DIR ${BERSEKER_APP_ROOT_DIR}/src)

set(
    BERSEKER_APP_SOURCES
    ${BERSEKER_APP_SOURCE_DIR}/main.cpp
    ${BERSEKER_APP_SOURCE_DIR}/Scenes/Scene1.h
    ${BERSEKER_APP_SOURCE_DIR}/Scenes/Scene1.cpp
)

add_executable(
    ${BERSEKER_APP_BINARY}
    ${BERSEKER_APP_SOURCES}
)

target_link_libraries(
    ${BERSEKER_APP_BINARY}
    BersekerEngine
)