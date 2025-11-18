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
