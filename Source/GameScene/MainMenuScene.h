#pragma once
#include "GameScene.h"
#include "../Audio/AudioSystem.h"

class Texture;

class MainMenuScene : public GameScene
{
public:
    MainMenuScene(class Game* game);
    ~MainMenuScene();

    void Enter() override;
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* keyState) override;
    void RenderBackground() override;
    void Render() override;
    void Exit() override;

    SceneType GetType() const override { return SceneType::MAIN_MENU; }
    const char* GetName() const override { return "MainMenu"; }

private:
    void UpdateMenuTextures();

    Texture* mBackgroundTexture;
    int mSelectedOption;
    bool mKeyWasPressed;
    bool mConfirming;

    Texture* mOptionStartTexture;
    Texture* mOptionDebugTexture;
    Texture* mOptionExitTexture;

    SoundHandle mMenuSound;
};

