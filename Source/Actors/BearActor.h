#pragma once
#include "AnimatedCharacterActor.h"

class BearActor : public AnimatedCharacterActor
{
public:
    BearActor(Game* game);
    ~BearActor();

    EnemyStats GetEnemyStats() const override;
    std::vector<Card*> CreateEnemyDeck() const override;
};
