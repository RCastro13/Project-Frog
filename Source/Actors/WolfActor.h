#pragma once
#include "AnimatedCharacterActor.h"

class WolfActor : public AnimatedCharacterActor
{
public:
    WolfActor(Game* game);
    ~WolfActor();

    EnemyStats GetEnemyStats() const override;
    std::vector<Card*> CreateEnemyDeck() const override;
};
