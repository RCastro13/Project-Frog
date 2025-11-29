#pragma once
#include "GameScene.h"

class RestScene : public GameScene
{
public:
    RestScene(Game* game);
    ~RestScene();

    void Enter() override;
    void Update(float deltaTime) override;
    void RenderBackground() override;
    void Render() override;
    void ProcessInput(const Uint8* keyState) override;
    void Exit() override;

    SceneType GetType() const override { return SceneType::REST; }
    const char* GetName() const override { return "Rest"; }

private:
    class Texture* mBackgroundTexture;

    class Texture* mTitleTexture;
    class Texture* mOption1Texture;
    class Texture* mOption2Texture;

    int mSelectedOption;  // 0 = descansar, 1 = meditar
    bool mKeyWasPressed;
};

