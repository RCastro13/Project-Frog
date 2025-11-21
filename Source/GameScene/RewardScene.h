#pragma once
#include "GameScene.h"

enum class RewardMode
{
    COMBAT_VICTORY,   // Após vencer combate: 3 cartas ou 30 moedas
    TREASURE_CHEST    // Nó do mapa: 1 carta (60%) ou moedas 50-80 (40%)
};

class RewardScene : public GameScene
{
public:
    RewardScene(Game* game, RewardMode mode = RewardMode::COMBAT_VICTORY);
    ~RewardScene();

    void Enter() override;
    void Update(float deltaTime) override;
    void RenderBackground() override;
    void Render() override;
    void ProcessInput(const Uint8* keyState) override;
    void Exit() override;

    SceneType GetType() const override { return SceneType::REWARD; }
    const char* GetName() const override { return "Reward"; }

private:
    enum class RewardType { CARD, COINS };

    RewardMode mMode;
    RewardType mRewardType;
    int mCoinsAmount;

    class Texture* mBackgroundTexture;
    class Texture* mTextTexture; // textura pro texto que aparece

    class ChestNPC* mChestNPC;
    class CoinNPC* mCoinNPC;

    bool mCoinSpawned; // para saber quando spawnar a moeda saindo do bau

    int mSelectedOption;  // 0-2 = cartas (combat) ou 0=aceitar/1=recusar (treasure) ou 0=coletar (coins)
    bool mKeyWasPressed;

    void DetermineReward();
    void GiveRewardToPlayer();

    void DecideRewardSpawnLogic(float deltatime); // função para fazer as animações das coisas dinâmicas dessa tela
    void SpawnText(std::string text); // função pra spawnar o texto na tela
};

