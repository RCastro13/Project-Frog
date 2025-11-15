#pragma once
#include "Combatant.h"
#include "../Combat/Item.h"
#include "../Combat/Card.h"
#include "../Random.h"
#include "../Math.h"

#include <vector>
#include <deque>

class Enemy : public Combatant {
public:
    // difficulty en [0.0, 1.0], default 0.33
    Enemy(Game* game, std::string name, int health, int maxHealth,
          std::vector<Card*> deck = std::vector<Card*>(), double difficulty = 0.33);

    Card* SelectCard();

    void LearnFromAtack(Card* card);

    void Death() override;

    void SetDifficulty(double d);
    double GetDifficulty() const { return mDifficulty; }

private:
    double mDifficulty = 0.33; // 0.0 fácil (muita aleatoriedade) .. 1.0 difícil (quase sem erros)
    double mEpsilon = 0.3;     // probabilidade de jogar aleatório (mapa desde difficulty)

    // memória das últimas cartas do oponente (tamanho = mMemorySize)
    std::deque<Card*> mRecentOpponentCards;
    int mMemorySize = 1; // N entre 1 e 4
};
