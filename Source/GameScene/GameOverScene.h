#pragma once
#include "GameScene.h"

class Texture;

class GameOverScene : public GameScene
{
public:
    GameOverScene(class Game* game);
    ~GameOverScene();

    void Enter() override;
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* keyState) override;
    void RenderBackground() override;
    void Render() override;
    void Exit() override;

    SceneType GetType() const override { return SceneType::GAME_OVER; }
    const char* GetName() const override { return "GameOver"; }

private:
    Texture* mBackgroundTexture;
    Texture* mMenuTexture;
    float mPulseTimer;
    bool mKeyWasPressed;
};

