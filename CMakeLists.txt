cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 17)

set(PROJECT_NAME "Project-Frog")
project(${PROJECT_NAME})

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image REQUIRED)

set(GLEW_ROOT "C:/Program Files/GLEW")
set(GLEW_INCLUDE_DIRS "${GLEW_ROOT}/include")
set(GLEW_LIBRARIES "${GLEW_ROOT}/lib/Release/x64/glew32.lib")

include_directories(${GLEW_INCLUDE_DIRS})

include_directories(
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIR}
        ${GLEW_INCLUDE_DIRS}
)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

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
        Source/Map/MapNodeType.h
        Source/Map/MapNode.h
        Source/Map/MapNode.cpp
        Source/Map/MapGenerator.h
        Source/Map/MapGenerator.cpp
        Source/Combat/Card.h
        Source/Combat/Card.cpp
        Source/Actors/Combatant.h
        Source/Actors/Combatant.cpp
        Source/Actors/Player.h
        Source/Combat/Item.h
        Source/Actors/Player.cpp
        Source/Combat/CombatManager.h
        Source/Combat/CombatManager.cpp
        Source/Actors/Enemy.h
        Source/Actors/Enemy.cpp
)

# Libs
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ${GLEW_LIBRARIES}
        SDL2::SDL2main
        SDL2_image::SDL2_image
        SDL2::SDL2
        opengl32
)