#include "DebugMenuScene.h"
#include "MainMenuScene.h"
#include "GameOverScene.h"
#include "VictoryScene.h"
#include "AnimationTestScene.h"
#include "RewardScene.h"
#include "ShopScene.h"
#include "RestScene.h"
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
    , mOptionRewardTexture(nullptr)
    , mOptionShopTexture(nullptr)
    , mOptionRestTexture(nullptr)
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
    if (mOptionRewardTexture) {
        delete mOptionRewardTexture;
    }
    if (mOptionShopTexture) {
        delete mOptionShopTexture;
    }
    if (mOptionRestTexture) {
        delete mOptionRestTexture;
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
    if (!mBackgroundTexture) {
        mBackgroundTexture = mGame->GetRenderer()->GetTexture("../Assets/Background/Menu/menu.png");
    }

    UpdateMenuTextures();
}

void DebugMenuScene::UpdateMenuTextures()
{

    Vector3 goldColor(1.0f, 0.84f, 0.0f);
    Vector3 whiteColor(1.0f, 1.0f, 1.0f);

    if (!mGame->GetFont()) {
        SDL_Log("ERROR: Font is null in UpdateMenuTextures()!");
        return;
    }

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
    if (mOptionRewardTexture) {
        delete mOptionRewardTexture;
        mOptionRewardTexture = nullptr;
    }
    if (mOptionShopTexture) {
        delete mOptionShopTexture;
        mOptionShopTexture = nullptr;
    }
    if (mOptionRestTexture) {
        delete mOptionRestTexture;
        mOptionRestTexture = nullptr;
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

    Vector3 rewardColor = (mSelectedOption == 3) ? goldColor : whiteColor;
    mOptionRewardTexture = mGame->GetFont()->RenderText(
        "Recompensa (teste)",
        rewardColor,
        18,
        400
    );

    Vector3 shopColor = (mSelectedOption == 4) ? goldColor : whiteColor;
    mOptionShopTexture = mGame->GetFont()->RenderText(
        "Loja (teste)",
        shopColor,
        18,
        400
    );

    Vector3 restColor = (mSelectedOption == 5) ? goldColor : whiteColor;
    mOptionRestTexture = mGame->GetFont()->RenderText(
        "Descanso (teste)",
        restColor,
        18,
        400
    );

    Vector3 backColor = (mSelectedOption == 6) ? goldColor : whiteColor;
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
        mSelectedOption = (mSelectedOption + 1) % 7;
        UpdateMenuTextures();
        mKeyWasPressed = true;
    }
    else if ((keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_W]) && !mKeyWasPressed)
    {
        mSelectedOption = (mSelectedOption - 1 + 7) % 7;
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
        } else if (mSelectedOption == 3) {
            mGame->SetScene(new RewardScene(mGame, RewardMode::COMBAT_VICTORY));
        } else if (mSelectedOption == 4) {
            mGame->SetScene(new ShopScene(mGame));
        } else if (mSelectedOption == 5) {
            mGame->SetScene(new RestScene(mGame));
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

    if (mOptionRewardTexture) {
        mGame->GetRenderer()->DrawTexture(
            Vector2(320.0f, startY + optionSpacing * 3),
            Vector2(mOptionRewardTexture->GetWidth(), mOptionRewardTexture->GetHeight()),
            0.0f,
            Vector3(1.0f, 1.0f, 1.0f),
            mOptionRewardTexture,
            Vector4::UnitRect,
            Vector2::Zero
        );
    }

    if (mOptionShopTexture) {
        mGame->GetRenderer()->DrawTexture(
            Vector2(320.0f, startY + optionSpacing * 4),
            Vector2(mOptionShopTexture->GetWidth(), mOptionShopTexture->GetHeight()),
            0.0f,
            Vector3(1.0f, 1.0f, 1.0f),
            mOptionShopTexture,
            Vector4::UnitRect,
            Vector2::Zero
        );
    }

    if (mOptionRestTexture) {
        mGame->GetRenderer()->DrawTexture(
            Vector2(320.0f, startY + optionSpacing * 5),
            Vector2(mOptionRestTexture->GetWidth(), mOptionRestTexture->GetHeight()),
            0.0f,
            Vector3(1.0f, 1.0f, 1.0f),
            mOptionRestTexture,
            Vector4::UnitRect,
            Vector2::Zero
        );
    }

    if (mOptionBackTexture) {
        mGame->GetRenderer()->DrawTexture(
            Vector2(320.0f, startY + optionSpacing * 6),
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
    if (mOptionRewardTexture) {
        delete mOptionRewardTexture;
        mOptionRewardTexture = nullptr;
    }
    if (mOptionShopTexture) {
        delete mOptionShopTexture;
        mOptionShopTexture = nullptr;
    }
    if (mOptionRestTexture) {
        delete mOptionRestTexture;
        mOptionRestTexture = nullptr;
    }
    if (mOptionBackTexture) {
        delete mOptionBackTexture;
        mOptionBackTexture = nullptr;
    }

    mBackgroundTexture = nullptr;
}
