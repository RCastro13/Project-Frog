#pragma once
#include "GameScene.h"

class BlackScreenScene : public GameScene
{
public:
    BlackScreenScene(class Game* game);
    ~BlackScreenScene();

    void Enter() override;
    void Update(float deltaTime) override;
    void RenderBackground() override;
    void ProcessInput(const Uint8* keyState) override;
    void Exit() override;

    SceneType GetType() const override { return SceneType::BLACK_SCREEN; }
    const char* GetName() const override { return "BlackScreen"; }
};

