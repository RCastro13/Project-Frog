#include "GameOverScene.h"
#include "MainMenuScene.h"
#include "../Game.h"
#include "../Renderer/Texture.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/Font.h"
#include "../Math.h"
#include <SDL.h>

GameOverScene::GameOverScene(Game* game)
    : GameScene(game)
    , mBackgroundTexture(nullptr)
    , mMenuTexture(nullptr)
    , mPulseTimer(0.0f)
    , mKeyWasPressed(false)
    , mConfirming(false)
{
}

GameOverScene::~GameOverScene()
{
    if (mMenuTexture) {
        delete mMenuTexture;
        mMenuTexture = nullptr;
    }
}

void GameOverScene::Enter()
{
    mStateTime = 0.0f;
    mPulseTimer = 0.0f;
    mConfirming = false;

    SDL_SetWindowTitle(mGame->GetWindow(), "Project Frog");

    mBackgroundTexture = mGame->GetRenderer()->GetTexture("../Assets/Background/Menu/gameover.png");

    mMenuTexture = mGame->GetFont()->RenderText(
        "Menu",
        Vector3(1.0f, 0.84f, 0.0f),
        24,
        400
    );
}

void GameOverScene::Update(float deltaTime)
{
    mStateTime += deltaTime;
    mPulseTimer += deltaTime;

    UpdateFade(deltaTime);
}

void GameOverScene::ProcessInput(const Uint8* keyState)
{
    if (mFadeAlpha > 0.0f || mGame->IsFading())
        return;

    if ((keyState[SDL_SCANCODE_RETURN] || keyState[SDL_SCANCODE_SPACE]) && !mKeyWasPressed && !mConfirming)
    {
        mConfirming = true;
        mGame->ClearMap();
        mGame->SetScene(new MainMenuScene(mGame));
        mKeyWasPressed = true;
    }
    else if (!keyState[SDL_SCANCODE_RETURN] && !keyState[SDL_SCANCODE_SPACE])
    {
        mKeyWasPressed = false;
    }
}

void GameOverScene::RenderBackground()
{
    if (mBackgroundTexture)
    {
        mGame->GetRenderer()->DrawTexture(
            Vector2(320.0f, 224.0f),
            Vector2(640.0f, 448.0f),
            0.0f,
            Vector3(1.0f, 1.0f, 1.0f),
            mBackgroundTexture,
            Vector4::UnitRect,
            Vector2::Zero
        );
    }
}

void GameOverScene::Render()
{
    RenderBackground();

    if (mMenuTexture)
    {
        float pulseSpeed = 1.5f;
        float pulseAmount = 0.1f;
        float scale = 0.8f + Math::Sin(mPulseTimer * pulseSpeed) * pulseAmount;

        Vector2 position(320.0f, 365.0f);
        Vector2 size(mMenuTexture->GetWidth() * scale, mMenuTexture->GetHeight() * scale);

        mGame->GetRenderer()->DrawTexture(
            position,
            size,
            0.0f,
            Vector3(1.0f, 0.84f, 0.0f),
            mMenuTexture,
            Vector4::UnitRect,
            Vector2::Zero
        );
    }

    RenderFade();
}

void GameOverScene::Exit()
{
    if (mMenuTexture)
    {
        mMenuTexture->Unload();
        delete mMenuTexture;
        mMenuTexture = nullptr;
    }

    mBackgroundTexture = nullptr;
}

