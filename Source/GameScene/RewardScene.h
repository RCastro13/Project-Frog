#pragma once
#include "GameScene.h"
#include "../Combat/Card.h"

enum class RewardMode
{
    COMBAT_VICTORY,   // Após vencer combate: 3 cartas ou 30 moedas
    TREASURE_CHEST,    // Nó do mapa: 1 carta (60%) ou moedas 50-80 (40%)
    SHOP_TREASURE_CHEST // Baús vindo da loja
};

// definições de constantes locais para UI (tipo no CombatConstants)
namespace RewardConstants {
    const float CARD_WIDTH = 80.0f;
    const float CARD_HEIGHT = 110.0f;
    const float SPACING = 10.0f;
    const float SELECTED_OFFSET = -20.0f;
    const float DECK_Y = 300.0f;
}

namespace FontSizes
{
    constexpr int POWER_SMALL = 24;
    constexpr int POWER_LARGE = 36;
    constexpr int HP_TEXT = 16;
}

namespace Offsets
{
    constexpr float POWER_TEXT_FROM_BOTTOM = 30.0f;
    constexpr float POWER_TEXT_FROM_BOTTOM_LARGE = 44.0f;
    constexpr float HP_TEXT_FROM_BAR = 18.0f;
    constexpr float HP_BAR_Y_OFFSET = 160.0f;
    constexpr float WINNER_FRAME_EXTRA = 20.0f;
    constexpr float WINNER_FRAME_Y_OFFSET = -5.0f;
}

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

    // Estados internos da cena
    enum class SceneState {
        ANIMATING_CHEST,      // Baú abrindo / Moedas girando
        SHOWING_REWARD,       // Mostra a carta ganha ou total de moedas
        CHOOSING_REPLACEMENT  // Jogador está escolhendo qual carta substituir
    };

    RewardMode mMode;
    RewardType mRewardType;
    SceneState mState; // Novo controle de estado

    int mCoinsAmount;
    Card* mRewardCard; // A nova carta gerada

    class Texture* mBackgroundTexture;
    class Texture* mTextTexture; // textura pro texto que aparece

    class ChestNPC* mChestNPC;
    class CoinNPC* mCoinNPC;

    // Texturas das cartas (Copiado de CombatScene)
    std::map<AttackType, class Texture*> mCardTextures;
    class Texture* mCardSelectedHighlight; // Opcional: textura para destacar seleção

    bool mCoinSpawned; // para saber quando spawnar a moeda saindo do bau

    int mSelectedDeckIndex; // Qual carta do deck do player está selecionada para troca
    bool mKeyWasPressed;

    void DetermineReward();
    void GiveRewardToPlayer();
    void GenerateRewardCard(); // Gera uma carta aleatória
    void DecideRewardSpawnLogic(float deltatime); // função para fazer as animações das coisas dinâmicas dessa tela
    void SpawnText(std::string text); // função pra spawnar o texto na tela
    void RenderCardValueText(Vector2 pos, int value, Vector3 color, int fontSize);
    void RenderCardInfo(Card* card, Vector2 customPos = Vector2(0.0f, 0.0f), Vector2 customCardSize = Vector2(0.0f, 0.0f));

    // Helpers visuais
    void LoadCardTextures();
    class Texture* GetCardTexture(AttackType type);
    void RenderRewardCard(); // Renderiza a carta ganha no centro
    void RenderDeckSelection(); // Renderiza o deck para troca
};

