#pragma once
#include "EnemyType.h"

class Game;
class AnimatedCharacterActor;

class EnemyFactory
{
public:
    static AnimatedCharacterActor* CreateRandomEnemy(Game* game);
    static AnimatedCharacterActor* CreateEnemy(Game* game, EnemyType type);
    static EnemyType GetRandomEnemyType();
};

