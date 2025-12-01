#include "VictoryScene.h"
#include "MainMenuScene.h"
#include "../Game.h"
#include "../Renderer/Texture.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/Font.h"
#include "../Math.h"
#include <SDL.h>

VictoryScene::VictoryScene(Game* game)
    : GameScene(game)
    , mBackgroundTexture(nullptr)
    , mPlayAgainTexture(nullptr)
    , mPulseTimer(0.0f)
    , mKeyWasPressed(false)
    , mConfirming(false)
{
}

VictoryScene::~VictoryScene()
{
    if (mPlayAgainTexture) {
        delete mPlayAgainTexture;
        mPlayAgainTexture = nullptr;
    }
}

void VictoryScene::Enter()
{
    mStateTime = 0.0f;
    mPulseTimer = 0.0f;
    mConfirming = false;

    SDL_SetWindowTitle(mGame->GetWindow(), "Project Frog");

    mBackgroundTexture = mGame->GetRenderer()->GetTexture("../Assets/Background/Menu/youwin.png");

    mPlayAgainTexture = mGame->GetFont()->RenderText(
        "Menu",
        Vector3(1.0f, 0.84f, 0.0f),
        20,
        400
    );
}

void VictoryScene::Update(float deltaTime)
{
    mStateTime += deltaTime;
    mPulseTimer += deltaTime;

    UpdateFade(deltaTime);
}

void VictoryScene::ProcessInput(const Uint8* keyState)
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

void VictoryScene::RenderBackground()
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

void VictoryScene::Render()
{
    RenderBackground();

    if (mPlayAgainTexture)
    {
        float pulseSpeed = 1.5f;
        float pulseAmount = 0.1f;
        float scale = 0.8f + Math::Sin(mPulseTimer * pulseSpeed) * pulseAmount;

        Vector2 position(320.0f, 365.0f);
        Vector2 size(mPlayAgainTexture->GetWidth() * scale, mPlayAgainTexture->GetHeight() * scale);

        mGame->GetRenderer()->DrawTexture(
            position,
            size,
            0.0f,
            Vector3(1.0f, 0.84f, 0.0f),
            mPlayAgainTexture,
            Vector4::UnitRect,
            Vector2::Zero
        );
    }

    RenderFade();
}

void VictoryScene::Exit()
{
    if (mPlayAgainTexture)
    {
        mPlayAgainTexture->Unload();
        delete mPlayAgainTexture;
        mPlayAgainTexture = nullptr;
    }

    mBackgroundTexture = nullptr;
}

