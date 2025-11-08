cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 17)

set(PROJECT_NAME "Project-Frog")
project(${PROJECT_NAME})

# SDL2 principal
set(SDL2_DIR "C:/Program Files/SDL2")
find_package(SDL2 REQUIRED)

# SDL2_image manualmente
set(SDL2_IMAGE_ROOT "C:/Program Files/SDL2_IMAGE")
set(SDL2_IMAGE_INCLUDE_DIR "${SDL2_IMAGE_ROOT}/include")
set(SDL2_IMAGE_LIBRARY "${SDL2_IMAGE_ROOT}/lib/x64/SDL2_image.lib")

# GLEW
set(GLEW_ROOT "C:/Program Files/GLEW")
set(GLEW_INCLUDE_DIRS "${GLEW_ROOT}/include")
set(GLEW_LIBRARIES "${GLEW_ROOT}/lib/Release/x64/glew32.lib")

include_directories(
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIR}
        ${GLEW_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME}
        Source/Renderer/Shader.cpp
        Source/Renderer/Shader.h
        Source/Main.cpp
        Source/Math.cpp
        Source/Math.h
        Source/Random.cpp
        Source/Random.h
        Source/Game.cpp
        Source/Game.h
        Source/Actors/Actor.cpp
        Source/Actors/Actor.h
        Source/Components/Component.cpp
        Source/Components/Component.h
        Source/Components/Drawing/DrawComponent.cpp
        Source/Components/Drawing/DrawComponent.h
        Source/Components/Physics/RigidBodyComponent.cpp
        Source/Components/Physics/RigidBodyComponent.h
        Source/Components/Physics/AABBColliderComponent.cpp
        Source/Components/Physics/AABBColliderComponent.h
        Source/Components/ParticleSystemComponent.cpp
        Source/Components/ParticleSystemComponent.h
        Source/Renderer/Renderer.cpp
        Source/Renderer/Renderer.h
        Source/Renderer/VertexArray.h
        Source/Renderer/VertexArray.cpp
        Source/Renderer/Texture.cpp
        Source/Renderer/Texture.h
        Source/Components/Drawing/AnimatorComponent.cpp
        Source/Components/Drawing/AnimatorComponent.h
        Source/Components/Drawing/RectComponent.cpp
        Source/Components/Drawing/RectComponent.h
        Source/Interface/UIScreen.h
        Source/Interface/UIText.h
        Source/Interface/UIButton.h
        Source/Interface/UIImage.h
        Source/Interface/HUD/HUD.h
        Source/Audio/AudioSystem.h
        Source/GameScene/GameScene.h
        Source/Interface/UIButton.cpp
        Source/Interface/UIImage.cpp
        Source/Interface/UIScreen.cpp
        Source/Interface/UIText.cpp
        Source/GameScene/GameScene.cpp
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARY}
        ${GLEW_LIBRARIES}
        opengl32
)
