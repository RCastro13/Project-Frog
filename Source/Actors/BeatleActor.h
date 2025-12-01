#pragma once
#include "AnimatedCharacterActor.h"

class BeatleActor : public AnimatedCharacterActor
{
public:
    BeatleActor(Game* game);
    ~BeatleActor();

    EnemyStats GetEnemyStats() const override;
    std::vector<Card*> CreateEnemyDeck() const override;
};
