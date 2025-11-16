#pragma once
#include "EnemyType.h"

class Game;
class AnimatedCharacterActor;

class EnemyFactory
{
public:
    struct EnemyStats
    {
        int health;
        int maxHealth;
        double difficulty;
    };

    static AnimatedCharacterActor* CreateRandomEnemy(Game* game);
    static AnimatedCharacterActor* CreateEnemy(Game* game, EnemyType type);
    static EnemyType GetRandomEnemyType();
    static EnemyStats GetStatsForType(EnemyType type);
};

