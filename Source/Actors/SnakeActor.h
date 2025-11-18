#pragma once
#include "AnimatedCharacterActor.h"

class SnakeActor : public AnimatedCharacterActor
{
public:
    SnakeActor(Game* game);
    ~SnakeActor();

    EnemyStats GetEnemyStats() const override;
    std::vector<Card*> CreateEnemyDeck() const override;
};
