#include "EnemyFactory.h"
#include "BearActor.h"
#include "BeatleActor.h"
#include "SnakeActor.h"
#include "WolfActor.h"
#include "GolemBossActor.h"
#include "../Random.h"
#include "../Game.h"
#include "../Map/MapNode.h"
#include <algorithm>

AnimatedCharacterActor* EnemyFactory::CreateRandomEnemy(Game* game)
{
    EnemyType type = GetRandomEnemyType(game);
    return CreateEnemy(game, type);
}

AnimatedCharacterActor* EnemyFactory::CreateEnemy(Game* game, EnemyType type)
{
    switch (type)
    {
        case EnemyType::BEAR:   return new BearActor(game);
        case EnemyType::BEATLE: return new BeatleActor(game);
        case EnemyType::SNAKE:  return new SnakeActor(game);
        case EnemyType::WOLF:   return new WolfActor(game);
        case EnemyType::GOLEM:  return new GolemBossActor(game);
        default:                return new BearActor(game);
    }
}

EnemyType EnemyFactory::GetRandomEnemyType(Game* game)
{
    const std::vector<MapNode*>& nodes = game->GetMapNodes();
    MapNode* currentNode = game->GetCurrentMapNode();

    if (nodes.empty() || !currentNode) {
        // Se algo der errado (teste sem mapa), retorna um aleatório básico
        int randomIndex = Random::GetIntRange(0, static_cast<int>(EnemyType::WOLF));
        return static_cast<EnemyType>(randomIndex);
    }

    // descobre qual é a última camada
    int maxLayer = 0;
    for (MapNode* node : nodes) {
        // Usa o GetLayer() que vimos no seu MapNode.h
        if (node->GetLayer() > maxLayer) {
            maxLayer = node->GetLayer();
        }
    }

    // evita divisão por zero caso o mapa tenha tamanho 1 (improvável, mas seguro)
    if (maxLayer == 0) maxLayer = 1;

    // calcula o progresso (0.0 a 1.0)
    float progress = static_cast<float>(currentNode->GetLayer()) / static_cast<float>(maxLayer);

    std::vector<EnemyType> possibleEnemies;

    // lógica de balanceamento baseada na metade do mapa
    if (progress <= 0.5f) { // Primeira metade (Início -> Meio) só renderiza cobra ou besouro
        possibleEnemies.push_back(EnemyType::BEATLE);
        possibleEnemies.push_back(EnemyType::SNAKE);
    } else { // Segunda metade (Meio -> Final) só renderiza lobo ou urso
        possibleEnemies.push_back(EnemyType::BEAR);
        possibleEnemies.push_back(EnemyType::WOLF);
    }

    // 5. Sorteia um inimigo da lista filtrada
    int index = Random::GetIntRange(0, static_cast<int>(possibleEnemies.size()) - 1);

    return possibleEnemies[index];
}