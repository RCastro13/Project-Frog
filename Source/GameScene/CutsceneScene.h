#pragma once
#include "GameScene.h"
#include <vector>
#include <string>
#include "../Audio/AudioSystem.h"

class Texture;

class CutsceneScene : public GameScene
{
public:
    CutsceneScene(class Game* game);
    ~CutsceneScene();

    void Enter() override;
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* keyState) override;
    void RenderBackground() override;
    void Render() override;
    void Exit() override;

    SceneType GetType() const override { return SceneType::CUTSCENE; }
    const char* GetName() const override { return "Cutscene"; }

private:
    void CreateTextLines();

    SoundHandle mCutsceneMusic;
    float mSoundTimer;
    class Texture* mBackgroundTexture;
    float mScrollOffset;
    float mScrollSpeed;
    std::vector<std::string> mNarrativeLines;
    std::vector<class Texture*> mTextTextures;
    std::vector<bool> mLineIsSpecial; // Para linhas com cor dourada
    bool mKeyWasPressed;
    bool mConfirming;
    class Texture* mSkipTexture;
    float mTotalTextHeight;
};

