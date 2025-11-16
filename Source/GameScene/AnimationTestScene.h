#pragma once
#include "GameScene.h"
#include <vector>

class AnimatedCharacterActor;

class AnimationTestScene : public GameScene
{
public:
    AnimationTestScene(Game* game);
    ~AnimationTestScene();

    void Enter() override;
    void Update(float deltaTime) override;
    void RenderBackground() override;
    void Render() override;
    void ProcessInput(const Uint8* keyState) override;
    void Exit() override;

    SceneType GetType() const override { return SceneType::MAIN_MENU; }
    const char* GetName() const override { return "AnimationTest"; }

private:
    enum class AnimationType
    {
        IDLE,
        ATTACK,
        HURT,
        DEATH,
        COUNT
    };

    void CreateTestActors();
    void PlayAnimation(AnimationType type);
    void RenderInstructions();

    class Texture* mBackgroundTexture;
    std::vector<AnimatedCharacterActor*> mTestActors;
    AnimationType mCurrentAnimation;
    bool mKeyWasPressed;
};
