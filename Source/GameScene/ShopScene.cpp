#include "ShopScene.h"
#include "../Game.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/Texture.h"
#include "../Actors/Player.h"
#include "../Actors/Combatant.h"
#include "../Actors/RabbitNPC.h"
#include "../Random.h"
#include <SDL.h>
#include <cstdio>

ShopScene::ShopScene(Game* game)
    : GameScene(game)
    , mBackgroundTexture(nullptr)
    , mRabbitNPC(nullptr)
    , mSelectedOption(0)
    , mKeyWasPressed(false)
{
}

ShopScene::~ShopScene()
{
}

void ShopScene::Enter()
{
    mStateTime = 0.0f;
    mFadeAlpha = 1.0f;
    mFadeTimer = 0.0f;
    mFadingIn = true;

    // Carregar background
    mBackgroundTexture = mGame->GetRenderer()->GetTexture("../Assets/Background/Shop/shop.png");
    if (!mBackgroundTexture)
    {
        // Fallback
        mBackgroundTexture = mGame->GetRenderer()->GetTexture("../Assets/Background/Menu/menu.png");
    }

    // Criar e posicionar o NPC Rabbit (vendedor)
    mRabbitNPC = new RabbitNPC(mGame);
    mRabbitNPC->SetPosition(Vector2(440.0f, 340.0f));
    mRabbitNPC->PlayIdle();

    GenerateItems();

    // NOTA: Sistema de moedas implementado no Player, mas ainda não há acesso global
    // Por enquanto, o sistema funciona via logs
    SDL_Log("[SHOP] Bem-vindo a loja!");
    SDL_Log("[SHOP] Itens disponiveis:");
    for (size_t i = 0; i < mItems.size(); i++)
    {
        SDL_Log("  [%zu] %s - %d moedas - %s",
                i + 1,
                mItems[i].name.c_str(),
                mItems[i].price,
                mItems[i].description.c_str());
    }
    SDL_Log("  [0] Sair da loja");
}

void ShopScene::GenerateItems()
{
    mItems.clear();

    // Gerar 4-6 itens aleatórios
    int numItems = Random::GetIntRange(4, 6);

    for (int i = 0; i < numItems; i++)
    {
        int itemRoll = Random::GetIntRange(0, 3);
        ShopItem item;

        switch (itemRoll)
        {
            case 0: // Carta
                item.name = "Carta Rara";
                item.description = "Nova carta para o deck";
                item.price = Random::GetIntRange(30, 50);
                item.type = ItemType::CARD;
                break;

            case 1: // Upgrade
                item.name = "Upgrade de Carta";
                item.description = "+1 Dano ou -1 Cooldown";
                item.price = 40;
                item.type = ItemType::UPGRADE_CARD;
                break;

            case 2: // Poção de HP
                item.name = "Pocao de Vida";
                item.description = "+5 HP";
                item.price = 25;
                item.type = ItemType::HEAL_POTION;
                break;

            case 3: // Aumento de HP máx
                item.name = "Amuleto de Vitalidade";
                item.description = "+5 HP Maximo";
                item.price = 60;
                item.type = ItemType::MAX_HP_INCREASE;
                break;
        }

        mItems.push_back(item);
    }
}

void ShopScene::Update(float deltaTime)
{
    mStateTime += deltaTime;
    UpdateFade(deltaTime);
}

void ShopScene::ProcessInput(const Uint8* keyState)
{
    // Bloquear inputs durante fade
    if (mFadeAlpha > 0.0f)
        return;

    // Tecla 0 para sair
    if (keyState[SDL_SCANCODE_0] && !mKeyWasPressed)
    {
        SDL_Log("[SHOP] Player saiu da loja");
        mKeyWasPressed = true;
        mGame->SetScene(new MapScene(mGame));
        return;
    }

    // Teclas 1-6 para comprar itens
    for (size_t i = 0; i < mItems.size() && i < 6; i++)
    {
        SDL_Scancode key = (SDL_Scancode)(SDL_SCANCODE_1 + i);
        if (keyState[key] && !mKeyWasPressed)
        {
            PurchaseItem(i);
            mKeyWasPressed = true;
            return;
        }
    }

    // Reset do flag
    bool anyKeyPressed = keyState[SDL_SCANCODE_0];
    for (int i = 0; i < 6; i++)
    {
        anyKeyPressed = anyKeyPressed || keyState[SDL_SCANCODE_1 + i];
    }

    if (!anyKeyPressed)
    {
        mKeyWasPressed = false;
    }
}

void ShopScene::PurchaseItem(int index)
{
    if (index < 0 || index >= static_cast<int>(mItems.size()))
        return;

    ShopItem& item = mItems[index];

    SDL_Log("[SHOP] Comprado: %s por %d moedas", item.name.c_str(), item.price);
    // NOTA: Player.SpendCoins() já existe, mas precisa de acesso global ao Player

    // Aplicar efeito do item
    switch (item.type)
    {
        case ItemType::CARD:
            SDL_Log("[SHOP] Nova carta adicionada ao deck!");
            // TODO: Implementar sistema de adicionar carta ao deck
            break;

        case ItemType::UPGRADE_CARD:
            SDL_Log("[SHOP] Carta melhorada!");
            // TODO: Implementar sistema de upgrade de carta
            break;

        case ItemType::HEAL_POTION:
            SDL_Log("[SHOP] Pocao de vida comprada (+5 HP)");
            // Player.SetHealth() disponível, precisa acesso global
            break;

        case ItemType::MAX_HP_INCREASE:
            SDL_Log("[SHOP] Amuleto de vitalidade comprado (+5 HP max)");
            // Player.SetMaxHealth() disponível, precisa acesso global
            break;
    }

    // Remover item da loja (compra única)
    mItems.erase(mItems.begin() + index);

    // Atualizar lista no console
    SDL_Log("[SHOP] Itens restantes:");
    for (size_t i = 0; i < mItems.size(); i++)
    {
        SDL_Log("  [%zu] %s - %d moedas", i + 1, mItems[i].name.c_str(), mItems[i].price);
    }
    if (mItems.empty())
    {
        SDL_Log("[SHOP] Loja vazia! Pressione 0 para sair");
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
    // UI: Sistema de Font existe, mas UI visual da loja ainda não implementada
    RenderFade();
}

void ShopScene::Exit()
{
    // Destruir o NPC
    if (mRabbitNPC)
    {
        mRabbitNPC->SetState(ActorState::Destroy);
        mRabbitNPC = nullptr;
    }

    mBackgroundTexture = nullptr;
}

