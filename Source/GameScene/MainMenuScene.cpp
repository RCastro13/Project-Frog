#include "MainMenuScene.h"
#include "MapScene.h"
#include "BlackScreenScene.h"
#include "DebugMenuScene.h"
#include "../Game.h"
#include "../Renderer/Texture.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/Font.h"
#include "../Math.h"
#include <SDL.h>

constexpr bool SHOW_DEBUG_OPTION = false;

MainMenuScene::MainMenuScene(Game* game)
    : GameScene(game)
    , mBackgroundTexture(nullptr)
    , mSelectedOption(0)
    , mKeyWasPressed(false)
    , mConfirming(false)
    , mOptionStartTexture(nullptr)
    , mOptionDebugTexture(nullptr)
    , mOptionExitTexture(nullptr)
{
}

MainMenuScene::~MainMenuScene()
{
    if (mOptionStartTexture) {
        delete mOptionStartTexture;
    }
    if (mOptionDebugTexture) {
        delete mOptionDebugTexture;
    }
    if (mOptionExitTexture) {
        delete mOptionExitTexture;
    }
}

void MainMenuScene::Enter()
{
    mStateTime = 0.0f;
    mConfirming = false;

    SDL_SetWindowTitle(mGame->GetWindow(), "Project Frog");

    mBackgroundTexture = mGame->GetRenderer()->GetTexture("../Assets/Background/Menu/menu.png");

    UpdateMenuTextures();

    mMenuSound = mGame->GetAudio()->PlaySound("MainMenu.wav", true);

    mGame->InitializeActors();
}

void MainMenuScene::UpdateMenuTextures()
{
    Vector3 goldColor(1.0f, 0.84f, 0.0f);
    Vector3 whiteColor(1.0f, 1.0f, 1.0f);

    if (!mGame->GetFont()) return;

    if (mOptionStartTexture) {
        delete mOptionStartTexture;
        mOptionStartTexture = nullptr;
    }
    if (mOptionDebugTexture) {
        delete mOptionDebugTexture;
        mOptionDebugTexture = nullptr;
    }
    if (mOptionExitTexture) {
        delete mOptionExitTexture;
        mOptionExitTexture = nullptr;
    }

    Vector3 startColor = (mSelectedOption == 0) ? goldColor : whiteColor;
    mOptionStartTexture = mGame->GetFont()->RenderText("Jogar", startColor, 22, 400);

    if (SHOW_DEBUG_OPTION) {
        Vector3 debugColor = (mSelectedOption == 1) ? goldColor : whiteColor;
        mOptionDebugTexture = mGame->GetFont()->RenderText("Debug", debugColor, 22, 400);

        Vector3 exitColor = (mSelectedOption == 2) ? goldColor : whiteColor;
        mOptionExitTexture = mGame->GetFont()->RenderText("Sair", exitColor, 22, 400);
    } else {
        Vector3 exitColor = (mSelectedOption == 1) ? goldColor : whiteColor;
        mOptionExitTexture = mGame->GetFont()->RenderText("Sair", exitColor, 22, 400);
    }
}

void MainMenuScene::Update(float deltaTime)
{
    mStateTime += deltaTime;
    UpdateFade(deltaTime);
}

void MainMenuScene::ProcessInput(const Uint8* keyState)
{
    if (ShouldBlockInput())
        return;

    int totalOptions = SHOW_DEBUG_OPTION ? 3 : 2;

    if ((keyState[SDL_SCANCODE_DOWN] || keyState[SDL_SCANCODE_S]) && !mKeyWasPressed)
    {
        mSelectedOption = (mSelectedOption + 1) % totalOptions;
        UpdateMenuTextures();
        mKeyWasPressed = true;
        mGame->GetAudio()->PlaySound("ChangeOption.wav", false);
    }
    else if ((keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_W]) && !mKeyWasPressed)
    {
        mSelectedOption = (mSelectedOption - 1 + totalOptions) % totalOptions;
        UpdateMenuTextures();
        mKeyWasPressed = true;
        mGame->GetAudio()->PlaySound("ChangeOption.wav", false);
    }
    else if ((keyState[SDL_SCANCODE_RETURN] || keyState[SDL_SCANCODE_SPACE]) && !mKeyWasPressed && !mConfirming)
    {
        mConfirming = true;
        if (mSelectedOption == 0) {
            mGame->SetScene(new MapScene(mGame));
        } else if (SHOW_DEBUG_OPTION && mSelectedOption == 1) {
            mGame->SetScene(new DebugMenuScene(mGame));
        } else if ((!SHOW_DEBUG_OPTION && mSelectedOption == 1) || (SHOW_DEBUG_OPTION && mSelectedOption == 2)) {
            mGame->SetScene(new BlackScreenScene(mGame));
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

void MainMenuScene::RenderBackground()
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

void MainMenuScene::Render()
{
    float startY = 180.0f;
    float optionSpacing = 80.0f;

    if (mOptionStartTexture) {
        mGame->GetRenderer()->DrawTexture(
            Vector2(320.0f, startY),
            Vector2(mOptionStartTexture->GetWidth(), mOptionStartTexture->GetHeight()),
            0.0f,
            Vector3(1.0f, 1.0f, 1.0f),
            mOptionStartTexture,
            Vector4::UnitRect,
            Vector2::Zero
        );
    }

    if (SHOW_DEBUG_OPTION && mOptionDebugTexture) {
        mGame->GetRenderer()->DrawTexture(
            Vector2(320.0f, startY + optionSpacing),
            Vector2(mOptionDebugTexture->GetWidth(), mOptionDebugTexture->GetHeight()),
            0.0f,
            Vector3(1.0f, 1.0f, 1.0f),
            mOptionDebugTexture,
            Vector4::UnitRect,
            Vector2::Zero
        );
    }

    if (mOptionExitTexture) {
        float exitY = SHOW_DEBUG_OPTION ? (startY + optionSpacing * 2) : (startY + optionSpacing);
        mGame->GetRenderer()->DrawTexture(
            Vector2(320.0f, exitY),
            Vector2(mOptionExitTexture->GetWidth(), mOptionExitTexture->GetHeight()),
            0.0f,
            Vector3(1.0f, 1.0f, 1.0f),
            mOptionExitTexture,
            Vector4::UnitRect,
            Vector2::Zero
        );
    }

    RenderFade();
}

void MainMenuScene::Exit()
{
    if (mOptionStartTexture) {
        delete mOptionStartTexture;
        mOptionStartTexture = nullptr;
    }
    if (mOptionDebugTexture) {
        delete mOptionDebugTexture;
        mOptionDebugTexture = nullptr;
    }
    if (mOptionExitTexture) {
        delete mOptionExitTexture;
        mOptionExitTexture = nullptr;
    }

    mBackgroundTexture = nullptr;

    mGame->GetAudio()->StopSound(mMenuSound);
}

