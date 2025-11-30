#include "RestScene.h"
#include "../Game.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/Texture.h"
#include "../Renderer/Font.h"
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

    Font* font = mGame->GetFont();
    if (font)
    {
        // Título: Cor amarela/dourada, tamanho 28 (estimado), wrap 500
        mTitleTexture = font->RenderText(
            "Local de Descanso",
            Vector3(1.0f, 0.9f, 0.2f),
            28,
            500
        );

        // Opção 1: Branco, tamanho 20, wrap 500
        mOption1Texture = font->RenderText(
            "[1] Descansar (+15 HP)",
            Vector3(1.0f, 1.0f, 1.0f),
            20,
            500
        );

        // Opção 2: Branco, tamanho 20, wrap 500
        mOption2Texture = font->RenderText(
            "[2] Treinar (+10 Vida Max)",
            Vector3(1.0f, 1.0f, 1.0f),
            20,
            500
        );
    }
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
        int cura = 15;
        mGame->GetPlayer()->Heal(cura);

        SDL_Log("[REST] Voce descansou e recuperou %d HP. Vida atual: %d/%d",
            cura, mGame->GetPlayer()->GetHealth(), mGame->GetPlayer()->GetMaxHealth());
        // Player.SetHealth() disponível, precisa acesso global

        mKeyWasPressed = true;
        mGame->SetScene(new MapScene(mGame));
        return;
    }
    // Opção 2: Meditar (reduzir cooldown)
    else if (keyState[SDL_SCANCODE_2] && !mKeyWasPressed)
    {
        int bonus = 10;
        mGame->GetPlayer()->IncreaseMaxHealth(bonus);

        SDL_Log("[REST] Voce treinou! Vida Maxima aumentada em +%d. Vida atual: %d/%d",
            bonus, mGame->GetPlayer()->GetHealth(), mGame->GetPlayer()->GetMaxHealth());

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
    Renderer* renderer = mGame->GetRenderer();

    // Renderizar Título
    if (mTitleTexture)
    {
        renderer->DrawTexture(
            Vector2(320.0f, 100.0f), // Posição centralizada no X (meio da tela)
            Vector2(static_cast<float>(mTitleTexture->GetWidth()), static_cast<float>(mTitleTexture->GetHeight())),
            0.0f,
            Vector3(1.0f, 1.0f, 1.0f),
            mTitleTexture
        );
    }

    // Renderizar Opção 1
    if (mOption1Texture)
    {
        renderer->DrawTexture(
            Vector2(320.0f, 250.0f),
            Vector2(static_cast<float>(mOption1Texture->GetWidth()), static_cast<float>(mOption1Texture->GetHeight())),
            0.0f,
            Vector3(1.0f, 1.0f, 1.0f),
            mOption1Texture
        );
    }

    // Renderizar Opção 2
    if (mOption2Texture)
    {
        renderer->DrawTexture(
            Vector2(320.0f, 300.0f),
            Vector2(static_cast<float>(mOption2Texture->GetWidth()), static_cast<float>(mOption2Texture->GetHeight())),
            0.0f,
            Vector3(1.0f, 1.0f, 1.0f),
            mOption2Texture
        );
    }
    RenderFade();
}

void RestScene::Exit()
{
    mBackgroundTexture = nullptr;
}

