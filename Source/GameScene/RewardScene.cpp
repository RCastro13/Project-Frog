#include "RewardScene.h"
#include "../Game.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/Texture.h"
#include "../Actors/Player.h"
#include "../Random.h"
#include <SDL.h>

#include "../Actors/ChestNPC.h"
#include "../Actors/CoinNPC.h"

RewardScene::RewardScene(Game* game, RewardMode mode)
    : GameScene(game)
    , mMode(mode)
    , mRewardType(RewardType::CARD)
    , mCoinsAmount(0)
    , mBackgroundTexture(nullptr)
    , mSelectedOption(0)
    , mKeyWasPressed(false)
    , mCoinSpawned(false)
    , mTextTexture(nullptr)
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
    mCoinSpawned = false;

    // Carregar background
    mBackgroundTexture = mGame->GetRenderer()->GetTexture("../Assets/Background/Rewards/rewards.png");
    if (!mBackgroundTexture)
    {
        // Fallback
        mBackgroundTexture = mGame->GetRenderer()->GetTexture("../Assets/Background/Combat/floresta.png");
    }

    //criar e posicionar o bau
    mChestNPC = new ChestNPC(mGame);
    mChestNPC->SetPosition(Vector2(Game::WINDOW_WIDTH / 2, Game::WINDOW_HEIGHT / 2 + 50));
    mChestNPC->Open();

    DetermineReward();
}

void RewardScene::DecideRewardSpawnLogic(float deltatime) {
    //logica de spawn
    if (!mCoinSpawned && mChestNPC && mChestNPC->IsOpenFinished()) // Só vai entrar nesse if se o baú tiver terminado de abrir
    {
        mCoinSpawned = true; // Trava para não entrar aqui de novo

        if (mMode == RewardMode::COMBAT_VICTORY)
        {
            // spawno a animação da moeda girando
            mCoinNPC = new CoinNPC(mGame);
            mCoinNPC->SetPosition(Vector2(Game::WINDOW_WIDTH / 2, Game::WINDOW_HEIGHT / 2 - 100));

            SpawnText("Você ganhou 30 moedas!");

            SDL_Log("[REWARD] Bau abriu! Spawnando recompensas de combate.");
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

                // Se tiver animação visual da moeda saindo do baú, spawna aqui também
            }
        }
    }
}

void RewardScene::SpawnText(std::string text) {
    // Cor do texto
    Vector3 whiteColor(1.0f, 1.0f, 1.0f);

    if (!mGame->GetFont()) return;

    // Limpar texturas antigas
    if (mTextTexture) {
        delete mTextTexture;
        mTextTexture = nullptr;
    }

    // Criar textura do texto com cor apropriada
    mTextTexture = mGame->GetFont()->RenderText(
        text,
        whiteColor,
        22,
        400
    );
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

    DecideRewardSpawnLogic(deltaTime);
}

void RewardScene::ProcessInput(const Uint8* keyState)
{
    // Bloquear inputs durante fade
    if (mFadeAlpha > 0.0f)
        return;

    if (mMode == RewardMode::COMBAT_VICTORY)
    {
        if (keyState[SDL_SCANCODE_RETURN] && !mKeyWasPressed) {
            SDL_Log("[REWARD] Player ganhou 30 moedas");
            mGame->GetPlayer()->GiveCoins(30);
            // TODO: Implementar sistema de adicionar carta ao deck
            mKeyWasPressed = true;
            mGame->SetScene(new MapScene(mGame));
        }
        // Teclas 1-4 para escolher
        // if (keyState[SDL_SCANCODE_1] && !mKeyWasPressed)
        // {
        //     SDL_Log("[REWARD] Player escolheu Carta 1");
        //     // TODO: Implementar sistema de adicionar carta ao deck
        //     mKeyWasPressed = true;
        //     mGame->SetScene(new MapScene(mGame));
        // }
        // else if (keyState[SDL_SCANCODE_2] && !mKeyWasPressed)
        // {
        //     SDL_Log("[REWARD] Player escolheu Carta 2");
        //     mKeyWasPressed = true;
        //     mGame->SetScene(new MapScene(mGame));
        // }
        // else if (keyState[SDL_SCANCODE_3] && !mKeyWasPressed)
        // {
        //     SDL_Log("[REWARD] Player escolheu Carta 3");
        //     mKeyWasPressed = true;
        //     mGame->SetScene(new MapScene(mGame));
        // }
        // else if (keyState[SDL_SCANCODE_4] && !mKeyWasPressed)
        // {
        //     SDL_Log("[REWARD] Player escolheu 30 moedas");
        //     GiveRewardToPlayer();
        //     mKeyWasPressed = true;
        //     mGame->SetScene(new MapScene(mGame));
        // }
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
    if (mTextTexture) {
        // Posição vertical base
        float startY = 180.0f;

        mGame->GetRenderer()->DrawTexture(
            Vector2(320.0f, startY),
            Vector2(mTextTexture->GetWidth(), mTextTexture->GetHeight()),
            0.0f,
            Vector3(1.0f, 1.0f, 1.0f),
            mTextTexture,
            Vector4::UnitRect,
            Vector2::Zero
        );
    }

    RenderFade();
}

void RewardScene::Exit()
{
    mBackgroundTexture = nullptr;
    mTextTexture = nullptr;
}

