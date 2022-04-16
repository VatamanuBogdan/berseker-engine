cmake_minimum_required(VERSION 3.1)

set(IMGUI_LIBRARY_BIN imgui)

set(IMGUI_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(IMGUI_INCLUDE_DIR ${IMGUI_ROOT_DIR}/include)
set(IMGUI_SOURCE_DIR ${IMGUI_ROOT_DIR}/imgui-module)

set(IMGUI_HEADERS
    ${IMGUI_SOURCE_DIR}/imconfig.h
    ${IMGUI_SOURCE_DIR}/imgui.h
    ${IMGUI_SOURCE_DIR}/imgui_internal.h
    ${IMGUI_SOURCE_DIR}/imstb_rectpack.h
    ${IMGUI_SOURCE_DIR}/imstb_textedit.h
    ${IMGUI_SOURCE_DIR}/imstb_truetype.h
    ${IMGUI_SOURCE_DIR}/backends/imgui_impl_opengl3.h
    ${IMGUI_SOURCE_DIR}/backends/imgui_impl_glfw.h
    )

set(IMGUI_CPPS
    ${IMGUI_SOURCE_DIR}/imgui.cpp
    ${IMGUI_SOURCE_DIR}/imgui_demo.cpp
    ${IMGUI_SOURCE_DIR}/imgui_draw.cpp
    ${IMGUI_SOURCE_DIR}/imgui_tables.cpp
    ${IMGUI_SOURCE_DIR}/imgui_widgets.cpp
    ${IMGUI_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    )

file(COPY ${IMGUI_HEADERS} DESTINATION ${IMGUI_INCLUDE_DIR})

set(IMGUI_SOURCES ${IMGUI_HEADERS} ${IMGUI_CPPS})

add_library(${IMGUI_LIBRARY_BIN} ${IMGUI_SOURCES})

target_link_libraries(${IMGUI_LIBRARY_BIN} glfw)

target_include_directories(${IMGUI_LIBRARY_BIN} PUBLIC ${IMGUI_INCLUDE_DIR})