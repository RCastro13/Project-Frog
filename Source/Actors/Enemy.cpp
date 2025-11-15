#include "Enemy.h"
#include <algorithm>
#include <climits>

// Função auxiliar: decide o tamanho da memória (N) a partir da dificuldade
static int MemorySizeFromDifficulty(double difficulty) {
    // Mapeamento simples:
    // difficulty <= 0.33 -> N = 1
    // 0.33 < difficulty <= 0.66 -> N = 2
    // 0.66 < difficulty <= 0.90 -> N = 3
    // difficulty > 0.90 -> N = 4
    if (difficulty <= 0.33) return 1;
    if (difficulty <= 0.66) return 2;
    if (difficulty <= 0.90) return 3;
    return 4;
}

Enemy::Enemy(Game* game, std::string name, int health, int maxHealth,
             std::vector<Card*> deck, double difficulty)
    : Combatant(game, name, health, maxHealth, deck)
{
    mDifficulty = Math::Clamp(difficulty, 0.0, 1.0);
    mMemorySize = MemorySizeFromDifficulty(mDifficulty);

    // Mapear difficulty -> epsilon (probabilidade de erro)
    // difficulty=0 -> epsilon ~0.6 (muito azar)
    // difficulty=1 -> epsilon ~0.02 (quase nenhum erro)
    mEpsilon = Math::Clamp(0.6 * (1.0 - mDifficulty) + 0.02, 0.02, 0.62);

    // Limpar memória inicial
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

    // 3) Calcular TYPE_BIAS dinamicamente com base no dano máximo do deck (evita constantes mágicas)
    int maxDamage = 1;
    for (Card* c : deck) if (c) maxDamage = std::max(maxDamage, c->GetDamage());
    int typeBias = Math::Max(1, maxDamage * 10); // bias proporcional ao dano máximo

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

// Aprende (guarda) a carta do oponente na memória de tamanho N
void Enemy::LearnFromAtack(Card* card) {
    if (!card) return;

    // Adicionar ao final
    mRecentOpponentCards.push_back(card);

    // Se exceder o tamanho, remover a mais antiga
    while ((int)mRecentOpponentCards.size() > mMemorySize) {
        mRecentOpponentCards.pop_front();
    }
}

// Comportamento ao morrer
void Enemy::Death() {
    Combatant::Death();
    // (futuro: dropar itens, efeitos visuais, etc.)
}

// Altera a dificuldade (reconfigura epsilon e tamanho da memória)
void Enemy::SetDifficulty(double d) {
    mDifficulty = Math::Clamp(d, 0.0, 1.0);
    mEpsilon = Math::Clamp(0.6 * (1.0 - mDifficulty) + 0.02, 0.02, 0.62);

    int newSize = MemorySizeFromDifficulty(mDifficulty);
    if (newSize != mMemorySize) {
        mMemorySize = newSize;
        // Se a memória atual for maior que o novo tamanho, recortar
        while ((int)mRecentOpponentCards.size() > mMemorySize) mRecentOpponentCards.pop_front();
    }
}
