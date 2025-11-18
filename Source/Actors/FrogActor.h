#pragma once
#include "AnimatedCharacterActor.h"

class FrogActor : public AnimatedCharacterActor
{
public:
    FrogActor(Game* game);
    ~FrogActor();

    // FrogActor é do player, não inimigo - métodos retornam valores padrão
    EnemyStats GetEnemyStats() const override;
    std::vector<Card*> CreateEnemyDeck() const override;
};
