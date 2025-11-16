#include "EnemyFactory.h"
#include "BearActor.h"
#include "BeatleActor.h"
#include "SnakeActor.h"
#include "WolfActor.h"
#include "../Random.h"
#include "../Game.h"

AnimatedCharacterActor* EnemyFactory::CreateRandomEnemy(Game* game)
{
    EnemyType type = GetRandomEnemyType();
    return CreateEnemy(game, type);
}

AnimatedCharacterActor* EnemyFactory::CreateEnemy(Game* game, EnemyType type)
{
    switch (type)
    {
        case EnemyType::BEAR:
            return new BearActor(game);

        case EnemyType::BEATLE:
            return new BeatleActor(game);

        case EnemyType::SNAKE:
            return new SnakeActor(game);

        case EnemyType::WOLF:
            return new WolfActor(game);

        default:
            return new BearActor(game);
    }
}

EnemyType EnemyFactory::GetRandomEnemyType()
{
    int randomIndex = Random::GetIntRange(0, static_cast<int>(EnemyType::COUNT) - 1);
    return static_cast<EnemyType>(randomIndex);
}

EnemyFactory::EnemyStats EnemyFactory::GetStatsForType(EnemyType type)
{
    switch (type)
    {
        case EnemyType::BEAR:
            return {30, 30, 0.35};  // HP 30, Difficulty 0.35 (balanceado)

        case EnemyType::BEATLE:
            return {25, 25, 0.30};  // HP 25, Difficulty 0.30 (mais fraco, menos esperto)

        case EnemyType::SNAKE:
            return {28, 28, 0.40};  // HP 28, Difficulty 0.40 (rápido e inteligente)

        case EnemyType::WOLF:
            return {32, 32, 0.38};  // HP 32, Difficulty 0.38 (mais forte)

        default:
            return {30, 30, 0.35};  // Default: Bear stats
    }
}
