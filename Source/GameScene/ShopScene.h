#pragma once
#include "GameScene.h"
#include "../Audio/AudioSystem.h"
#include <vector>
#include <string>

class ShopScene : public GameScene
{
public:
    ShopScene(Game* game);
    ~ShopScene();

    void Enter() override;
    void Update(float deltaTime) override;
    void RenderBackground() override;
    void Render() override;
    void ProcessInput(const Uint8* keyState) override;
    void Exit() override;

    SceneType GetType() const override { return SceneType::SHOP; }
    const char* GetName() const override { return "Shop"; }

private:
    enum class ItemType
    {
        HEAL_POTION,
        MAX_HP_INCREASE,
        MYSTERY_CHEST
    };

    struct ShopItem
    {
        std::string name;
        int price;
        ItemType type;

        // Texturas cacheadas para renderização otimizada
        class Texture* nameTexture = nullptr;
        class Texture* descTexture = nullptr;
        class Texture* priceTexture = nullptr;
    };

    SoundHandle mShopMusic;

    class Texture* mBackgroundTexture;
    class RabbitNPC* mRabbitNPC;  // NPC vendedor

    // Textura para mostrar o saldo de moedas do jogador
    class Texture* mPlayerCoinTexture;
    class Texture* mPlayerHPTexture;

    std::vector<ShopItem> mItems;
    int mSelectedOption;  // 0 a 2
    bool mKeyWasPressed;

    void GenerateItems();
    void PurchaseItem(int index);

    // Auxiliar para atualizar a textura de moedas quando gastamos dinheiro
    void UpdateCoinTexture();
    void UpdateHPTexture();
};

