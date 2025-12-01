#pragma once
#include "GameScene.h"

class DebugMenuScene : public GameScene
{
public:
    DebugMenuScene(Game* game);
    ~DebugMenuScene();

    void Enter() override;
    void Update(float deltaTime) override;
    void RenderBackground() override;
    void Render() override;
    void ProcessInput(const Uint8* keyState) override;
    void Exit() override;

    SceneType GetType() const override { return SceneType::MAIN_MENU; }
    const char* GetName() const override { return "DebugMenu"; }

private:
    void UpdateMenuTextures();

    class Texture* mBackgroundTexture;

    int mSelectedOption;      // 0-6 = opções, 6 = Voltar
    bool mKeyWasPressed;

    class Texture* mOptionAnimTestTexture;
    class Texture* mOptionGameOverTexture;
    class Texture* mOptionVictoryTexture;
    class Texture* mOptionRewardTexture;
    class Texture* mOptionShopTexture;
    class Texture* mOptionRestTexture;
    class Texture* mOptionBackTexture;
};
