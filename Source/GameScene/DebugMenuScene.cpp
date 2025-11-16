#include "DebugMenuScene.h"
#include "AnimationTestScene.h"
#include "../Game.h"
#include "../Renderer/Texture.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/Font.h"
#include <SDL.h>

DebugMenuScene::DebugMenuScene(Game* game)
    : GameScene(game)
    , mBackgroundTexture(nullptr)
    , mSelectedOption(0)
    , mKeyWasPressed(false)
    , mOptionAnimTestTexture(nullptr)
    , mOptionGameOverTexture(nullptr)
    , mOptionVictoryTexture(nullptr)
    , mOptionBackTexture(nullptr)
{
}

DebugMenuScene::~DebugMenuScene()
{
    if (mOptionAnimTestTexture) {
        delete mOptionAnimTestTexture;
    }
    if (mOptionGameOverTexture) {
        delete mOptionGameOverTexture;
    }
    if (mOptionVictoryTexture) {
        delete mOptionVictoryTexture;
    }
    if (mOptionBackTexture) {
        delete mOptionBackTexture;
    }
}

void DebugMenuScene::Enter()
{
    mStateTime = 0.0f;
    mFadeAlpha = 1.0f;
    mFadeTimer = 0.0f;
    mFadingIn = true;

    mBackgroundTexture = mGame->GetRenderer()->GetTexture("../Assets/Background/Menu/debug.jpeg");

    UpdateMenuTextures();
}

void DebugMenuScene::UpdateMenuTextures()
{
    Vector3 goldColor(1.0f, 0.84f, 0.0f);
    Vector3 whiteColor(1.0f, 1.0f, 1.0f);

    if (!mGame->GetFont()) return;

    if (mOptionAnimTestTexture) {
        delete mOptionAnimTestTexture;
        mOptionAnimTestTexture = nullptr;
    }
    if (mOptionGameOverTexture) {
        delete mOptionGameOverTexture;
        mOptionGameOverTexture = nullptr;
    }
    if (mOptionVictoryTexture) {
        delete mOptionVictoryTexture;
        mOptionVictoryTexture = nullptr;
    }
    if (mOptionBackTexture) {
        delete mOptionBackTexture;
        mOptionBackTexture = nullptr;
    }

    Vector3 animTestColor = (mSelectedOption == 0) ? goldColor : whiteColor;
    mOptionAnimTestTexture = mGame->GetFont()->RenderText(
        "Teste de Animacoes",
        animTestColor,
        18,
        400
    );

    Vector3 gameOverColor = (mSelectedOption == 1) ? goldColor : whiteColor;
    mOptionGameOverTexture = mGame->GetFont()->RenderText(
        "Game Over (teste)",
        gameOverColor,
        18,
        400
    );

    Vector3 victoryColor = (mSelectedOption == 2) ? goldColor : whiteColor;
    mOptionVictoryTexture = mGame->GetFont()->RenderText(
        "Vitoria (teste)",
        victoryColor,
        18,
        400
    );

    Vector3 backColor = (mSelectedOption == 3) ? goldColor : whiteColor;
    mOptionBackTexture = mGame->GetFont()->RenderText(
        "Voltar ao Menu",
        backColor,
        18,
        400
    );
}

void DebugMenuScene::Update(float deltaTime)
{
    mStateTime += deltaTime;
    UpdateFade(deltaTime);
}

void DebugMenuScene::ProcessInput(const Uint8* keyState)
{
    if (mFadeAlpha > 0.0f)
        return;

    if ((keyState[SDL_SCANCODE_DOWN] || keyState[SDL_SCANCODE_S]) && !mKeyWasPressed)
    {
        mSelectedOption = (mSelectedOption + 1) % 4;
        UpdateMenuTextures();
        mKeyWasPressed = true;
    }
    else if ((keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_W]) && !mKeyWasPressed)
    {
        mSelectedOption = (mSelectedOption - 1 + 4) % 4;
        UpdateMenuTextures();
        mKeyWasPressed = true;
    }
    else if ((keyState[SDL_SCANCODE_RETURN] || keyState[SDL_SCANCODE_SPACE]) && !mKeyWasPressed)
    {
        if (mSelectedOption == 0) {
            mGame->SetScene(new AnimationTestScene(mGame));
        } else if (mSelectedOption == 1) {
            mGame->SetScene(new class GameOverScene(mGame));
        } else if (mSelectedOption == 2) {
            mGame->SetScene(new class VictoryScene(mGame));
        } else {
            mGame->SetScene(new class MainMenuScene(mGame));
        }
        mKeyWasPressed = true;
    }
    else if (!keyState[SDL_SCANCODE_DOWN] && !keyState[SDL_SCANCODE_S] &&
             !keyState[SDL_SCANCODE_UP] && !keyState[SDL_SCANCODE_W] &&
             !keyState[SDL_SCANCODE_RETURN] && !keyState[SDL_SCANCODE_SPACE])
    {
        mKeyWasPressed = false;
    }
}

void DebugMenuScene::RenderBackground()
{
    if (mBackgroundTexture) {
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

void DebugMenuScene::Render()
{
    float startY = 165.0f;
    float optionSpacing = 45.0f;

    // Título
    if (mGame->GetFont()) {
        Texture* titleTexture = mGame->GetFont()->RenderText(
            "DEBUG MENU",
            Vector3(1.0f, 0.84f, 0.0f),
            28
        );
        if (titleTexture) {
            mGame->GetRenderer()->DrawTexture(
                Vector2(320.0f, 100.0f),
                Vector2(titleTexture->GetWidth(), titleTexture->GetHeight()),
                0.0f,
                Vector3(1.0f, 1.0f, 1.0f),
                titleTexture,
                Vector4::UnitRect,
                Vector2::Zero
            );
            delete titleTexture;
        }
    }

    if (mOptionAnimTestTexture) {
        mGame->GetRenderer()->DrawTexture(
            Vector2(320.0f, startY),
            Vector2(mOptionAnimTestTexture->GetWidth(), mOptionAnimTestTexture->GetHeight()),
            0.0f,
            Vector3(1.0f, 1.0f, 1.0f),
            mOptionAnimTestTexture,
            Vector4::UnitRect,
            Vector2::Zero
        );
    }

    if (mOptionGameOverTexture) {
        mGame->GetRenderer()->DrawTexture(
            Vector2(320.0f, startY + optionSpacing),
            Vector2(mOptionGameOverTexture->GetWidth(), mOptionGameOverTexture->GetHeight()),
            0.0f,
            Vector3(1.0f, 1.0f, 1.0f),
            mOptionGameOverTexture,
            Vector4::UnitRect,
            Vector2::Zero
        );
    }

    if (mOptionVictoryTexture) {
        mGame->GetRenderer()->DrawTexture(
            Vector2(320.0f, startY + optionSpacing * 2),
            Vector2(mOptionVictoryTexture->GetWidth(), mOptionVictoryTexture->GetHeight()),
            0.0f,
            Vector3(1.0f, 1.0f, 1.0f),
            mOptionVictoryTexture,
            Vector4::UnitRect,
            Vector2::Zero
        );
    }

    if (mOptionBackTexture) {
        mGame->GetRenderer()->DrawTexture(
            Vector2(320.0f, startY + optionSpacing * 3),
            Vector2(mOptionBackTexture->GetWidth(), mOptionBackTexture->GetHeight()),
            0.0f,
            Vector3(1.0f, 1.0f, 1.0f),
            mOptionBackTexture,
            Vector4::UnitRect,
            Vector2::Zero
        );
    }
}

void DebugMenuScene::Exit()
{
}
