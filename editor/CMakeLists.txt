cmake_minimum_required(VERSION 3.4)

# Parameters Setup

set(EDITOR_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(DEPENDENCIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies)
set(EDITOR_BINARY_NAME Editor)

# Sources Setup

set(
    EDITOR_RELATIVE_SOURCES
    main.cpp
    EditorApplication.cpp
    EditorApplication.h
    Core/UIRendererImpl_GL_GLFW.cpp
    Core/UIRendererImpl_GL_GLFW.h
    Core/UIRendererBackend.h
    Core/View.h
    Core/Layout.h
    Controllers/EditorCameraController.h
    Controllers/EditorCameraController.cpp
    Views/LinearLayout.h
    Views/LinearLayout.cpp 
    Views/LoggerView.hpp)

set(EDITOR_SOURCES "")

foreach(SOURCE ${EDITOR_RELATIVE_SOURCES})
    list(APPEND EDITOR_SOURCES "${EDITOR_SOURCES_DIR}/${SOURCE}")
endforeach()

# ImGUI Library Setup (https://github.com/ocornut/imgui)

add_subdirectory(${DEPENDENCIES_DIR}/imgui)

# Library Setup

add_executable(
    ${EDITOR_BINARY_NAME}
    ${EDITOR_SOURCES}
)

target_link_libraries(
    ${EDITOR_BINARY_NAME}
    Example
    Engine
    imgui
)

target_include_directories(
    ${EDITOR_BINARY_NAME}
    PUBLIC ${EDITOR_SOURCES_DIR}
)