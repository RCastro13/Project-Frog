#pragma once
#include "AnimatedCharacterActor.h"

class GolemBossActor : public AnimatedCharacterActor
{
public:
    GolemBossActor(Game* game);
    ~GolemBossActor();

    EnemyStats GetEnemyStats() const override;
    std::vector<Card*> CreateEnemyDeck() const override;
};

