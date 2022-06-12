cmake_minimum_required(VERSION 3.1)

set(BERSEKER_EDITOR_BINARY BersekerEditor)
set(BERSEKER_EDITOR_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(BERSEKER_EDITOR_SOURCE_DIR ${BERSEKER_EDITOR_ROOT_DIR}/src)
set(BERSEKER_EDITOR_DEPENDENCIES_DIR ${BERSEKER_EDITOR_ROOT_DIR}/deps)

set(
    BERSEKER_EDITOR_SOURCES
    ${BERSEKER_EDITOR_SOURCE_DIR}/main.cpp
    ${BERSEKER_EDITOR_SOURCE_DIR}/EditorApplication.cpp
    ${BERSEKER_EDITOR_SOURCE_DIR}/EditorApplication.h
    ${BERSEKER_EDITOR_SOURCE_DIR}/UI/Implementation/UIRendererImpl_GL_GLFW.cpp
    ${BERSEKER_EDITOR_SOURCE_DIR}/UI/Implementation/UIRendererImpl_GL_GLFW.h
    ${BERSEKER_EDITOR_SOURCE_DIR}/UI/UIRendererBackend.h
    ${BERSEKER_EDITOR_SOURCE_DIR}/EditorCameraController.h
    ${BERSEKER_EDITOR_SOURCE_DIR}/EditorCameraController.cpp
)

# Set imgui library
add_subdirectory(${BERSEKER_EDITOR_DEPENDENCIES_DIR}/imgui)

add_executable(
    ${BERSEKER_EDITOR_BINARY}
    ${BERSEKER_EDITOR_SOURCES}
)

target_link_libraries(
    ${BERSEKER_EDITOR_BINARY}
    BersekerTesting
    BersekerEngine
    imgui
)

target_include_directories(
    ${BERSEKER_EDITOR_BINARY}
    PUBLIC ${BERSEKER_EDITOR_SOURCE_DIR}/
)