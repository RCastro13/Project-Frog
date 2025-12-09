#pragma once
#include "GameScene.h"
#include "../Audio/AudioSystem.h"

class Texture;

class VictoryScene : public GameScene
{
public:
    VictoryScene(class Game* game);
    ~VictoryScene();

    void Enter() override;
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* keyState) override;
    void RenderBackground() override;
    void Render() override;
    void Exit() override;

    SceneType GetType() const override { return SceneType::VICTORY; }
    const char* GetName() const override { return "Victory"; }

private:
    SoundHandle mVictoryMusic;
    Texture* mBackgroundTexture;
    Texture* mPlayAgainTexture;
    float mPulseTimer;
    bool mKeyWasPressed;
    bool mConfirming;
};

