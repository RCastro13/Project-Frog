#include "ShopScene.h"
#include "MapScene.h"
#include "../Game.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/Texture.h"
#include "../Renderer/Font.h"
#include "../Actors/Player.h"
#include "../Actors/RabbitNPC.h"
#include "../GameScene/GameScene.h"
#include "../GameScene/RewardScene.h"
#include <SDL.h>
#include <string>

#include "RewardScene.h"

ShopScene::ShopScene(Game* game)
    : GameScene(game)
    , mBackgroundTexture(nullptr)
    , mRabbitNPC(nullptr)
    , mPlayerCoinTexture(nullptr)
    , mPlayerHPTexture(nullptr) // Inicializa nulo
    , mSelectedOption(0)
    , mKeyWasPressed(false)
{
}

ShopScene::~ShopScene()
{
    if (mPlayerCoinTexture) {
        delete mPlayerCoinTexture;
        mPlayerCoinTexture = nullptr;
    }
    if (mPlayerHPTexture) {
        delete mPlayerHPTexture;
        mPlayerHPTexture = nullptr;
    }
    for (auto& item : mItems) {
        if (item.nameTexture) {
            delete item.nameTexture;
            item.nameTexture = nullptr;
        }
        if (item.descTexture) {
            delete item.descTexture;
            item.descTexture = nullptr;
        }
        if (item.priceTexture) {
            delete item.priceTexture;
            item.priceTexture = nullptr;
        }
    }
}

void ShopScene::Enter()
{
    mStateTime = 0.0f;
    mFadeAlpha = 1.0f;
    mFadeTimer = 0.0f;
    mFadingIn = true;
    mSelectedOption = 0;

    // Carregar background
    mBackgroundTexture = mGame->GetRenderer()->GetTexture("../Assets/Background/Shop/shop.png");
    if (!mBackgroundTexture)
    {
        mBackgroundTexture = mGame->GetRenderer()->GetTexture("../Assets/Background/Menu/menu.png");
    }

    // Criar e posicionar o NPC Rabbit
    mRabbitNPC = new RabbitNPC(mGame);
    mRabbitNPC->SetPosition(Vector2(550.0f, 300.0f));
    mRabbitNPC->PlayIdle();

    GenerateItems();
    UpdateCoinTexture();
    UpdateHPTexture(); // Gera a textura de HP inicial

    SDL_Log("[SHOP] Bem-vindo a loja!");
}

void ShopScene::UpdateCoinTexture()
{
    if (mPlayerCoinTexture) {
        delete mPlayerCoinTexture;
        mPlayerCoinTexture = nullptr;
    }

    Player* player = mGame->GetPlayer();
    if (player && mGame->GetFont())
    {
        std::string coinText = "Moedas: " + std::to_string(player->GetCoins());
        // Cor amarela
        mPlayerCoinTexture = mGame->GetFont()->RenderText(coinText, Vector3(1.0f, 0.9f, 0.2f), 20);
    }
}

// Implementação da atualização do texto de HP
void ShopScene::UpdateHPTexture()
{
    if (mPlayerHPTexture) {
        delete mPlayerHPTexture;
        mPlayerHPTexture = nullptr;
    }

    Player* player = mGame->GetPlayer();
    if (player && mGame->GetFont())
    {
        // Formato: HP: 50/100
        std::string hpText = "HP: " + std::to_string(player->GetHealth()) + "/" + std::to_string(player->GetMaxHealth());
        // Cor Vermelha/Rosada para vida
        mPlayerHPTexture = mGame->GetFont()->RenderText(hpText, Vector3(1.0f, 0.4f, 0.4f), 20);
    }
}

void ShopScene::GenerateItems()
{
    mItems.clear();
    Font* font = mGame->GetFont();

    // --- ITEM 0: Cura (+10 Vida) ---
    ShopItem item1;
    item1.name = "Pocao de Vida";
    item1.price = 20;
    item1.type = ItemType::HEAL_POTION;
    if (font) {
        item1.nameTexture = font->RenderText(item1.name, Vector3(1,1,1), 20);
        item1.descTexture = font->RenderText("Recupera 10 HP", Vector3(0.8f, 0.8f, 0.8f), 16);
        item1.priceTexture = font->RenderText("$ " + std::to_string(item1.price), Vector3(1, 1, 0), 20);
    }
    mItems.push_back(item1);

    // --- ITEM 1: Upgrade (+5 Max HP) ---
    ShopItem item2;
    item2.name = "Cristal Vital";
    item2.price = 50;
    item2.type = ItemType::MAX_HP_INCREASE;
    if (font) {
        item2.nameTexture = font->RenderText(item2.name, Vector3(1,1,1), 20);
        item2.descTexture = font->RenderText("+5 Vida Maxima", Vector3(0.8f, 0.8f, 0.8f), 16);
        item2.priceTexture = font->RenderText("$ " + std::to_string(item2.price), Vector3(1, 1, 0), 20);
    }
    mItems.push_back(item2);

    // --- ITEM 2: Baú (Leva para RewardScene) ---
    ShopItem item3;
    item3.name = "Bau Misterioso";
    item3.price = 100;
    item3.type = ItemType::MYSTERY_CHEST;
    if (font) {
        item3.nameTexture = font->RenderText(item3.name, Vector3(1,1,1), 20);
        item3.descTexture = font->RenderText("??? Surpresa ???", Vector3(0.8f, 0.8f, 0.8f), 16);
        item3.priceTexture = font->RenderText("$ " + std::to_string(item3.price), Vector3(1, 1, 0), 20);
    }
    mItems.push_back(item3);
}

void ShopScene::Update(float deltaTime)
{
    mStateTime += deltaTime;
    UpdateFade(deltaTime);
}

void ShopScene::ProcessInput(const Uint8* keyState)
{
    if (mFadeAlpha > 0.0f) return;

    if ((keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_W]) && !mKeyWasPressed)
    {
        mSelectedOption--;
        if (mSelectedOption < 0) mSelectedOption = 0;
        mKeyWasPressed = true;
    }
    else if ((keyState[SDL_SCANCODE_DOWN] || keyState[SDL_SCANCODE_S]) && !mKeyWasPressed)
    {
        mSelectedOption++;
        if (mSelectedOption >= (int)mItems.size()) mSelectedOption = (int)mItems.size() - 1;
        mKeyWasPressed = true;
    }
    else if ((keyState[SDL_SCANCODE_RETURN] || keyState[SDL_SCANCODE_SPACE]) && !mKeyWasPressed)
    {
        PurchaseItem(mSelectedOption);
        mKeyWasPressed = true;
    }
    else if ((keyState[SDL_SCANCODE_ESCAPE] || keyState[SDL_SCANCODE_BACKSPACE]) && !mKeyWasPressed)
    {
        SDL_Log("[SHOP] Saindo da loja...");
        mKeyWasPressed = true;
        mGame->SetScene(new MapScene(mGame));
    }

    if (!keyState[SDL_SCANCODE_UP] && !keyState[SDL_SCANCODE_W] &&
        !keyState[SDL_SCANCODE_DOWN] && !keyState[SDL_SCANCODE_S] &&
        !keyState[SDL_SCANCODE_RETURN] && !keyState[SDL_SCANCODE_SPACE] &&
        !keyState[SDL_SCANCODE_ESCAPE])
    {
        mKeyWasPressed = false;
    }
}

void ShopScene::PurchaseItem(int index)
{
    if (index < 0 || index >= static_cast<int>(mItems.size())) return;

    ShopItem& item = mItems[index];
    Player* player = mGame->GetPlayer();

    if (!player) return;

    if (player->GetCoins() >= item.price)
    {
        player->SpendCoins(item.price);
        UpdateCoinTexture();

        SDL_Log("[SHOP] Comprou %s por %d moedas.", item.name.c_str(), item.price);

        switch (item.type)
        {
            case ItemType::HEAL_POTION:
                player->Heal(10);
                UpdateHPTexture(); // Atualiza HP na tela
                SDL_Log("       Curou 10 HP.");
                break;

            case ItemType::MAX_HP_INCREASE:
                player->IncreaseMaxHealth(5);
                UpdateHPTexture(); // Atualiza HP na tela
                SDL_Log("       Max HP aumentou em 5.");
                break;

            case ItemType::MYSTERY_CHEST:
                SDL_Log("       Abrindo bau misterioso!");
                mGame->SetScene(new RewardScene(mGame, RewardMode::SHOP_TREASURE_CHEST));
                return;
        }
    }
    else
    {
        SDL_Log("[SHOP] Moedas insuficientes! Voce tem %d, precisa de %d.", player->GetCoins(), item.price);
    }
}

void ShopScene::RenderBackground()
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

void ShopScene::Render()
{
    Renderer* renderer = mGame->GetRenderer();

    // 1. Renderizar Saldo de Moedas
    if (mPlayerCoinTexture)
    {
        renderer->DrawTexture(
            Vector2(175.0f, 50.0f),
            Vector2(mPlayerCoinTexture->GetWidth(), mPlayerCoinTexture->GetHeight()),
            0.0f, Vector3(1,1,1), mPlayerCoinTexture,
            Vector4::UnitRect, Vector2::Zero
        );
    }

    // 2. Renderizar HP do Player
    if (mPlayerHPTexture)
    {
        // Posicionado um pouco mais à direita das moedas
        renderer->DrawTexture(
            Vector2(450.0f, 50.0f),
            Vector2(mPlayerHPTexture->GetWidth(), mPlayerHPTexture->GetHeight()),
            0.0f, Vector3(1,1,1), mPlayerHPTexture,
            Vector4::UnitRect, Vector2::Zero
        );
    }

    // 3. Renderizar Instruções
    Font* font = mGame->GetFont();
    if (font) {
        Texture* instrTex = font->RenderText(
            "[ENTER] Comprar   [ESC] Sair",
            Vector3(1,1,1),
            14
        );

        if (instrTex) {
            renderer->DrawTexture(
                Vector2(320.0f, 380.0f),
                Vector2(instrTex->GetWidth(), instrTex->GetHeight()),
                0.0f, Vector3(1,1,1), instrTex
            );
            delete instrTex;
        }
    }

    // 4. Renderizar Itens
    float startX = 60.0f;
    float startY = 120.0f;
    float gapY = 70.0f;

    for (size_t i = 0; i < mItems.size(); i++)
    {
        float x = startX;
        float y = startY + (i * gapY);
        ShopItem& item = mItems[i];

        if (i == mSelectedOption && font)
        {
            Texture* arrowTex = font->RenderText(">", Vector3(1,1,0), 24);
            if (arrowTex) {
                renderer->DrawTexture(
                    Vector2(x - 20.0f, y),
                    Vector2(arrowTex->GetWidth(), arrowTex->GetHeight()),
                    0.0f, Vector3(1,1,1), arrowTex
                );
                delete arrowTex;
            }
        }

        if (item.nameTexture)
        {
            renderer->DrawTexture(
                Vector2(x+150.0f, y),
                Vector2(item.nameTexture->GetWidth(), item.nameTexture->GetHeight()),
                0.0f, (i == mSelectedOption ? Vector3(1,1,0) : Vector3(1,1,1)),
                item.nameTexture,
                Vector4::UnitRect,
                Vector2::Zero
            );
        }

        if (item.priceTexture)
        {
            renderer->DrawTexture(
                Vector2(x + 400.0f, y),
                Vector2(item.priceTexture->GetWidth(), item.priceTexture->GetHeight()),
                0.0f, Vector3(0.5f, 1.0f, 0.5f), item.priceTexture,
                Vector4::UnitRect,
                Vector2::Zero
            );
        }

        if (item.descTexture)
        {
            renderer->DrawTexture(
                Vector2(x + 150.0f, y + 25.0f),
                Vector2(item.descTexture->GetWidth(), item.descTexture->GetHeight()),
                0.0f, Vector3(1,1,1), item.descTexture,
                Vector4::UnitRect,
                Vector2::Zero
            );
        }
    }

    RenderFade();
}

void ShopScene::Exit()
{
    if (mRabbitNPC)
    {
        mRabbitNPC->SetState(ActorState::Destroy);
        mRabbitNPC = nullptr;
    }

    for (auto& item : mItems)
    {
        if (item.nameTexture) delete item.nameTexture;
        if (item.descTexture) delete item.descTexture;
        if (item.priceTexture) delete item.priceTexture;
    }
    mItems.clear();

    if (mPlayerCoinTexture)
    {
        delete mPlayerCoinTexture;
        mPlayerCoinTexture = nullptr;
    }

    // Deletar textura de HP
    if (mPlayerHPTexture)
    {
        delete mPlayerHPTexture;
        mPlayerHPTexture = nullptr;
    }

    mBackgroundTexture = nullptr;
}