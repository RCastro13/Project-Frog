#include "RewardScene.h"
#include "../Game.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/Texture.h"
#include "../Actors/Player.h"
#include "../Random.h"
#include <SDL.h>

RewardScene::RewardScene(Game* game, RewardMode mode)
    : GameScene(game)
    , mMode(mode)
    , mRewardType(RewardType::CARD)
    , mCoinsAmount(0)
    , mBackgroundTexture(nullptr)
    , mSelectedOption(0)
    , mKeyWasPressed(false)
{
}

RewardScene::~RewardScene()
{
}

void RewardScene::Enter()
{
    mStateTime = 0.0f;
    mFadeAlpha = 1.0f;
    mFadeTimer = 0.0f;
    mFadingIn = true;

    // Carregar background
    mBackgroundTexture = mGame->GetRenderer()->GetTexture("../Assets/Background/Rewards/rewards.png");
    if (!mBackgroundTexture)
    {
        // Fallback
        mBackgroundTexture = mGame->GetRenderer()->GetTexture("../Assets/Background/Combat/floresta.png");
    }

    DetermineReward();

    if (mMode == RewardMode::COMBAT_VICTORY)
    {
        SDL_Log("[REWARD] Vitoria em combate! Escolha:");
        SDL_Log("  [1] Carta 1 (Fire Strike)");
        SDL_Log("  [2] Carta 2 (Water Shield)");
        SDL_Log("  [3] Carta 3 (Plant Whip)");
        SDL_Log("  [4] Pegar 30 moedas");
    }
    else // TREASURE_CHEST
    {
        if (mRewardType == RewardType::CARD)
        {
            SDL_Log("[REWARD] Bau encontrado! Carta rara disponivel!");
            SDL_Log("  [1] Aceitar (trocar carta do deck)");
            SDL_Log("  [2] Recusar");
        }
        else
        {
            SDL_Log("[REWARD] Bau encontrado! %d moedas!", mCoinsAmount);
            SDL_Log("  [ENTER] Coletar moedas");
        }
    }
}

void RewardScene::DetermineReward()
{
    if (mMode == RewardMode::TREASURE_CHEST)
    {
        // 60% chance de carta, 40% chance de moedas
        int roll = Random::GetIntRange(0, 99);

        if (roll < 60)
        {
            mRewardType = RewardType::CARD;
        }
        else
        {
            mRewardType = RewardType::COINS;
            mCoinsAmount = Random::GetIntRange(50, 80);
        }
    }
    else // COMBAT_VICTORY
    {
        mRewardType = RewardType::CARD; // Sempre oferece cartas no modo combate
        mCoinsAmount = 30; // Valor fixo se escolher moedas
    }
}

void RewardScene::Update(float deltaTime)
{
    mStateTime += deltaTime;
    UpdateFade(deltaTime);
}

void RewardScene::ProcessInput(const Uint8* keyState)
{
    // Bloquear inputs durante fade
    if (mFadeAlpha > 0.0f)
        return;

    if (mMode == RewardMode::COMBAT_VICTORY)
    {
        // Teclas 1-4 para escolher
        if (keyState[SDL_SCANCODE_1] && !mKeyWasPressed)
        {
            SDL_Log("[REWARD] Player escolheu Carta 1");
            // TODO: Implementar sistema de adicionar carta ao deck
            mKeyWasPressed = true;
            mGame->SetScene(new MapScene(mGame));
        }
        else if (keyState[SDL_SCANCODE_2] && !mKeyWasPressed)
        {
            SDL_Log("[REWARD] Player escolheu Carta 2");
            mKeyWasPressed = true;
            mGame->SetScene(new MapScene(mGame));
        }
        else if (keyState[SDL_SCANCODE_3] && !mKeyWasPressed)
        {
            SDL_Log("[REWARD] Player escolheu Carta 3");
            mKeyWasPressed = true;
            mGame->SetScene(new MapScene(mGame));
        }
        else if (keyState[SDL_SCANCODE_4] && !mKeyWasPressed)
        {
            SDL_Log("[REWARD] Player escolheu 30 moedas");
            GiveRewardToPlayer();
            mKeyWasPressed = true;
            mGame->SetScene(new MapScene(mGame));
        }
    }
    else // TREASURE_CHEST
    {
        if (mRewardType == RewardType::CARD)
        {
            // Aceitar/Recusar carta
            if (keyState[SDL_SCANCODE_1] && !mKeyWasPressed)
            {
                SDL_Log("[REWARD] Player aceitou a carta do bau");
                // TODO: Implementar sistema de trocar carta
                mKeyWasPressed = true;
                mGame->SetScene(new MapScene(mGame));
            }
            else if (keyState[SDL_SCANCODE_2] && !mKeyWasPressed)
            {
                SDL_Log("[REWARD] Player recusou a carta do bau");
                mKeyWasPressed = true;
                mGame->SetScene(new MapScene(mGame));
            }
        }
        else
        {
            // Coletar moedas
            if ((keyState[SDL_SCANCODE_RETURN] || keyState[SDL_SCANCODE_SPACE]) && !mKeyWasPressed)
            {
                SDL_Log("[REWARD] Player coletou %d moedas do bau", mCoinsAmount);
                GiveRewardToPlayer();
                mKeyWasPressed = true;
                mGame->SetScene(new MapScene(mGame));
            }
        }
    }

    // Reset do flag
    if (!keyState[SDL_SCANCODE_1] && !keyState[SDL_SCANCODE_2] &&
        !keyState[SDL_SCANCODE_3] && !keyState[SDL_SCANCODE_4] &&
        !keyState[SDL_SCANCODE_RETURN] && !keyState[SDL_SCANCODE_SPACE])
    {
        mKeyWasPressed = false;
    }
}

void RewardScene::GiveRewardToPlayer()
{
    if (mRewardType == RewardType::COINS)
    {
        SDL_Log("[REWARD] Player ganhou %d moedas!", mCoinsAmount);
        // Player.GiveCoins() disponível, precisa acesso global
    }
}

void RewardScene::RenderBackground()
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

void RewardScene::Render()
{
    // UI: Sistema de Font existe, mas UI visual de recompensa ainda não implementada
    RenderFade();
}

void RewardScene::Exit()
{
    mBackgroundTexture = nullptr;
}

