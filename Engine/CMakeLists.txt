cmake_minimum_required(VERSION 3.5)

# Parameters Setup

set(ENGINE_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/)
set(DEPENDENCIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps/)
set(ENGINE_LIBRARY_NAME Engine)

# Sources Setup

set(
    ENGINE_RELATIVE_SOURCES
    Rendering/GraphicsAPI/VertexBuffer.hpp
    Rendering/GraphicsAPI/VertexArray.hpp
    Rendering/GraphicsAPI/IndexBuffer.hpp
    Rendering/GraphicsAPI/ShaderProgram.cpp
    Rendering/GraphicsAPI/ShaderProgram.h
    Rendering/GraphicsAPI/Texture2D.hpp
    Rendering/GraphicsAPI/OpenGL.h
    Rendering/GraphicsAPI/OpenGL.cpp
    Rendering/Model.cpp
    Rendering/Renderer.cpp
    Rendering/Renderer.h
    Rendering/Mesh.h
    Core/Window/Window.h
    Core/Window/GLFW/GLFWWindow.cpp
    Core/Window/GLFW/GLFWWindow.h
    Core/Input/InputKeys.h
    Core/Input/InputManager.h
    Application.cpp
    Application.h
    Scene/Scene.h
    Rendering/Primitives/Renderer.cpp
    Rendering/Primitives/Renderer.h
    Rendering/Primitives/Primitives.h
    Rendering/Utils.h
    Rendering/Utils.cpp
    Physics/Partitioning/BVH.cpp
    Physics/Partitioning/BVH.h
    Physics/Boundings/BVolumes.cpp
    Physics/Boundings/BVolumes.h
    Scene/Components/Transform.h
    Scene/Components/ColliderComponent.h
    Scene/Components/Identifier.h
    Scene/Components/Components.h
    Scene/Components/ColliderComponent.h
    Rendering/GraphicsAPI/FrameBuffer.cpp
    Rendering/GraphicsAPI/FrameBuffer.h
    Rendering/Material.hpp
    Scene/Components/LightSource.h
    ResourceLoaders/ShaderRegistry.cpp
    ResourceLoaders/ShaderRegistry.h
    ResourceLoaders/ShaderResources.cpp
    ResourceLoaders/ShaderResources.h
    Rendering/LightSourceBillboard.cpp
    Rendering/LightSourceBillboard.h
    Rendering/GraphicsAPI/CubeMapTexture.hpp
    Rendering/Skybox.cpp
    Rendering/Skybox.h
    Core/Logger.cpp
    Core/Logger.h
    Core/Memory.cpp
    Core/Memory.h
    Core/Debug.h
    Ecs/Component.h
    Ecs/Component.cpp
    Ecs/ComponentSet.h
    Ecs/Entity.cpp
    Ecs/Entity.h
    Ecs/Registry.cpp
    Ecs/Registry.h
    Core/DataStructures/SparseSet.cpp
    Core/DataStructures/SparseSet.h
    Scene/Components/ModelComponent.h
    Physics/ColliderLoader.cpp
    Physics/ColliderLoader.h
    Physics/Collider.cpp
    Physics/Collider.h
)

set(ENGINE_SOURCES "")

foreach(SOURCE ${ENGINE_RELATIVE_SOURCES})
    list(APPEND ENGINE_SOURCES "${ENGINE_SOURCES_DIR}/${SOURCE}")
endforeach()

# Common Setup

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# Spdlog Library Setup (https://github.com/gabime/spdlog)

set(SPDLOG_INSTALL OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_SHARED OFF CACHE BOOL "" FORCE)

add_subdirectory(${DEPENDENCIES_DIR}/spdlog)

# Glad Loader Setup

add_subdirectory(${DEPENDENCIES_DIR}/glad)

# Glfw Library Setup (https://github.com/glfw/glfw)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(${DEPENDENCIES_DIR}/glfw)

# Glm Library Setup (https://github.com/g-truc/glm)

set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(${DEPENDENCIES_DIR}/glm)

# Stb Library Setup (https://github.com/nothings/stb)

add_subdirectory(${DEPENDENCIES_DIR}/stb)

# Assimp Library Setup (https://github.com/assimp/assimp/tree/master)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(${DEPENDENCIES_DIR}/assimp)

# Library Setup

add_library(
    ${ENGINE_LIBRARY_NAME}
    ${ENGINE_SOURCES}
)

target_link_libraries(
    ${ENGINE_LIBRARY_NAME}
    glfw
    glad
    glm
    spdlog
    stb
    assimp
)

target_include_directories(
    ${ENGINE_LIBRARY_NAME}
    PUBLIC ${ENGINE_SOURCES_DIR}/
)