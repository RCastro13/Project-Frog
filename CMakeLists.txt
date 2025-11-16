cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 17)

set(PROJECT_NAME "Project-Frog")
project(${PROJECT_NAME})


# macOS - SDL2 via Frameworks (padrão do time)
set(CMAKE_PREFIX_PATH "/opt/homebrew")

# SDL2 Frameworks
find_library(SDL2_LIBRARY SDL2 PATHS /Library/Frameworks)
find_library(SDL2_IMAGE_LIBRARY SDL2_image PATHS /Library/Frameworks)
find_library(SDL2_TTF_LIBRARY SDL2_ttf PATHS /Library/Frameworks)
find_library(SDL2_MIXER_LIBRARY SDL2_mixer PATHS /Library/Frameworks)

# GLEW e OpenGL via Homebrew
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(
        /Library/Frameworks/SDL2.framework/Headers
        /Library/Frameworks/SDL2_image.framework/Headers
        /Library/Frameworks/SDL2_ttf.framework/Headers
        /Library/Frameworks/SDL2_mixer.framework/Headers
        ${GLEW_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIR}
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
        Source/Renderer/Font.h
        Source/Renderer/Font.cpp
        Source/Audio/AudioSystem.h
        Source/Audio/AudioSystem.cpp
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
        Source/Actors/FrogActor.h
        Source/Actors/FrogActor.cpp
        Source/Actors/BearActor.h
        Source/Actors/BearActor.cpp
)

# Libs
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_TTF_LIBRARY}
        ${SDL2_MIXER_LIBRARY}
        GLEW::GLEW
        OpenGL::GL
)