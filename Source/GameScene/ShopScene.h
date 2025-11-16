#pragma once
#include "GameScene.h"
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
        CARD,
        UPGRADE_CARD,
        HEAL_POTION,
        MAX_HP_INCREASE
    };

    struct ShopItem
    {
        std::string name;
        std::string description;
        int price;
        ItemType type;
    };

    class Texture* mBackgroundTexture;
    class RabbitNPC* mRabbitNPC;  // NPC vendedor

    std::vector<ShopItem> mItems;
    int mSelectedOption;  // índice do item selecionado (ou último = sair)
    bool mKeyWasPressed;

    void GenerateItems();
    void PurchaseItem(int index);
};

