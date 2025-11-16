#include "Enemy.h"
#include <algorithm>
#include <climits>

namespace EnemyConstants
{
    constexpr double DIFFICULTY_THRESHOLD_LOW = 0.33;
    constexpr double DIFFICULTY_THRESHOLD_MID = 0.66;
    constexpr double DIFFICULTY_THRESHOLD_HIGH = 0.90;

    constexpr double EPSILON_MIN = 0.02;
    constexpr double EPSILON_MAX = 0.62;
    constexpr double EPSILON_FACTOR = 0.6;

    constexpr int TYPE_BIAS_MULTIPLIER = 10;
}

static int MemorySizeFromDifficulty(double difficulty) {
    using namespace EnemyConstants;
    if (difficulty <= DIFFICULTY_THRESHOLD_LOW) return 1;
    if (difficulty <= DIFFICULTY_THRESHOLD_MID) return 2;
    if (difficulty <= DIFFICULTY_THRESHOLD_HIGH) return 3;
    return 4;
}

Enemy::Enemy(Game* game, std::string name, int health, int maxHealth,
             std::vector<Card*> deck, double difficulty)
    : Combatant(game, name, health, maxHealth, deck)
{
    using namespace EnemyConstants;
    mDifficulty = Math::Clamp(difficulty, 0.0, 1.0);
    mMemorySize = MemorySizeFromDifficulty(mDifficulty);

    mEpsilon = Math::Clamp(EPSILON_FACTOR * (1.0 - mDifficulty) + EPSILON_MIN,
                          EPSILON_MIN, EPSILON_MAX);

    mRecentOpponentCards.clear();
}

// Seleciona a carta a ser jogada
Card* Enemy::SelectCard() {
    // 1) Obter o deck e filtrar cartas disponíveis
    std::vector<Card*> deck = GetDeck();
    std::vector<Card*> available;
    available.reserve(deck.size());

    for (Card* c : deck) {
        if (c && c->IsAvailable()) available.push_back(c);
    }

    if (available.empty()) {
        if (!deck.empty()) return deck[0];
        return nullptr;
    }

    // 2) Com probabilidade mEpsilon, escolher uma carta aleatória (jogada “ruim”)
    if (Random::GetFloat() < static_cast<float>(mEpsilon)) {
        int idx = Random::GetIntRange(0, static_cast<int>(available.size()) - 1);
        return available[idx];
    }

    int maxDamage = 1;
    for (Card* c : deck) if (c) maxDamage = std::max(maxDamage, c->GetDamage());
    int typeBias = Math::Max(1, maxDamage * EnemyConstants::TYPE_BIAS_MULTIPLIER);

    // 4) Avaliar cada carta levando em conta as últimas N cartas do oponente
    int bestIndex = 0;
    long long bestScore = LLONG_MIN;

    for (int i = 0; i < (int)available.size(); ++i) {
        Card* myCard = available[i];
        long long score = myCard->GetDamage(); // valor base: dano

        // Contar quantas cartas na memória esta carta ganha/perde
        int wins = 0;
        int losses = 0;
        for (Card* mem : mRecentOpponentCards) {
            if (!mem) continue;
            if (myCard->HasTypeAdvantageOver(mem)) ++wins;
            else if (mem->HasTypeAdvantageOver(myCard)) ++losses;
        }

        // Ajustar o score conforme wins/losses usando TYPE_BIAS
        score += (long long)typeBias * (wins - losses);

        // Escolher a melhor pontuação (desempate aleatório)
        if (score > bestScore) {
            bestScore = score;
            bestIndex = i;
        } else if (score == bestScore) {
            if (Random::GetIntRange(0, 1) == 1)
                bestIndex = i;
        }
    }

    return available[bestIndex];
}

void Enemy::LearnFromAttack(Card* card) {
    if (!card) return;

    // Adicionar ao final
    mRecentOpponentCards.push_back(card);

    // Se exceder o tamanho, remover a mais antiga
    while ((int)mRecentOpponentCards.size() > mMemorySize) {
        mRecentOpponentCards.pop_front();
    }
}

void Enemy::Death() {
    Combatant::Death();
}

void Enemy::SetDifficulty(double d) {
    using namespace EnemyConstants;
    mDifficulty = Math::Clamp(d, 0.0, 1.0);
    mEpsilon = Math::Clamp(EPSILON_FACTOR * (1.0 - mDifficulty) + EPSILON_MIN,
                          EPSILON_MIN, EPSILON_MAX);

    int newSize = MemorySizeFromDifficulty(mDifficulty);
    if (newSize != mMemorySize) {
        mMemorySize = newSize;
        while ((int)mRecentOpponentCards.size() > mMemorySize)
            mRecentOpponentCards.pop_front();
    }
}
