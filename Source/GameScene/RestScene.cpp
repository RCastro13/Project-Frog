#include "RestScene.h"
#include "../Game.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/Texture.h"
#include "../Actors/Player.h"
#include <SDL.h>

RestScene::RestScene(Game* game)
    : GameScene(game)
    , mBackgroundTexture(nullptr)
    , mSelectedOption(0)
    , mKeyWasPressed(false)
{
}

RestScene::~RestScene()
{
}

void RestScene::Enter()
{
    mStateTime = 0.0f;
    mFadeAlpha = 1.0f;
    mFadeTimer = 0.0f;
    mFadingIn = true;

    // Carregar background
    mBackgroundTexture = mGame->GetRenderer()->GetTexture("../Assets/Background/Rest/rest.png");
    if (!mBackgroundTexture)
    {
        // Fallback
        mBackgroundTexture = mGame->GetRenderer()->GetTexture("../Assets/Background/Menu/menu.png");
    }

    SDL_Log("[REST] Local de descanso encontrado");
    SDL_Log("  [1] Descansar (+10 HP)");
    SDL_Log("  [2] Meditar (Reduzir cooldown de 1 carta)");
}

void RestScene::Update(float deltaTime)
{
    mStateTime += deltaTime;
    UpdateFade(deltaTime);
}

void RestScene::ProcessInput(const Uint8* keyState)
{
    // Bloquear inputs durante fade
    if (mFadeAlpha > 0.0f)
        return;

    // Opção 1: Descansar (+10 HP)
    if (keyState[SDL_SCANCODE_1] && !mKeyWasPressed)
    {
        SDL_Log("[REST] Voce descansou e recuperou +10 HP");
        // Player.SetHealth() disponível, precisa acesso global

        mKeyWasPressed = true;
        mGame->SetScene(new MapScene(mGame));
        return;
    }
    // Opção 2: Meditar (reduzir cooldown)
    else if (keyState[SDL_SCANCODE_2] && !mKeyWasPressed)
    {
        SDL_Log("[REST] Voce meditou e aprimorou suas habilidades");
        // Sistema de cooldown já existe no Card, precisa UI para seleção

        mKeyWasPressed = true;
        mGame->SetScene(new MapScene(mGame));
        return;
    }

    // Reset do flag
    if (!keyState[SDL_SCANCODE_1] && !keyState[SDL_SCANCODE_2])
    {
        mKeyWasPressed = false;
    }
}

void RestScene::RenderBackground()
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

void RestScene::Render()
{
    // UI: Sistema de Font existe, mas UI visual de descanso ainda não implementada
    RenderFade();
}

void RestScene::Exit()
{
    mBackgroundTexture = nullptr;
}

