#include "MapGenerator.h"
#include "../Random.h"
#include "../Math.h"
#include <SDL_log.h>

std::vector<MapNode*> MapGenerator::Generate(const MapConfig& config) {
    std::vector<MapNode*> allNodes;
    std::vector<std::vector<MapNode*>> layers;

    int numLayers = Random::GetIntRange(config.minLayers, config.maxLayers);
    int nodeID = 0;


    // Layer 0: Nó inicial (sempre 1)
    layers.push_back({});
    MapNode* startNode = new MapNode(nodeID++, MapNodeType::START, 0);
    layers[0].push_back(startNode);
    allNodes.push_back(startNode);

    // Layers intermediárias: tipos variados
    for (int layer = 1; layer < numLayers - 1; layer++) {
        int nodesInLayer = Random::GetIntRange(config.minNodesPerLayer, config.maxNodesPerLayer);
        layers.push_back({});

        for (int i = 0; i < nodesInLayer; i++) {
            MapNodeType type = SelectRandomType(layer, numLayers, config);
            MapNode* node = new MapNode(nodeID++, type, layer);
            layers[layer].push_back(node);
            allNodes.push_back(node);
        }
    }

    // Layer final: Boss (sempre 1)
    layers.push_back({});
    MapNode* bossNode = new MapNode(nodeID++, MapNodeType::BOSS, numLayers - 1);
    layers[numLayers - 1].push_back(bossNode);
    allNodes.push_back(bossNode);

    // Conectar as camadas
    ConnectLayers(layers, config);

    // Garantir que todos os nós são alcançáveis
    EnsureConnectivity(layers);

    // Calcular posições visuais
    CalculateNodePositions(allNodes, 640, 448);

    // Marcar nó inicial como acessível e atual
    startNode->SetAccessible(true);
    startNode->SetCurrent(true);


    return allNodes;
}

MapNodeType MapGenerator::SelectRandomType(int layer, int totalLayers, const MapConfig& config) {
    // Evitar lojas e descansos nas primeiras camadas
    if (layer < 2) {
        float rand = Random::GetFloat();
        if (rand < 0.7f) return MapNodeType::COMBAT;
        if (rand < 0.9f) return MapNodeType::TREASURE;
        return MapNodeType::ELITE;
    }

    // Distribuição normal para camadas intermediárias
    float rand = Random::GetFloat();
    float accumulated = 0.0f;

    accumulated += config.combatChance;
    if (rand < accumulated) return MapNodeType::COMBAT;

    accumulated += config.eliteChance;
    if (rand < accumulated) return MapNodeType::ELITE;

    accumulated += config.shopChance;
    if (rand < accumulated) return MapNodeType::SHOP;

    accumulated += config.treasureChance;
    if (rand < accumulated) return MapNodeType::TREASURE;

    return MapNodeType::REST;
}

void MapGenerator::ConnectLayers(std::vector<std::vector<MapNode*>>& layers, const MapConfig& config) {
    for (size_t i = 0; i < layers.size() - 1; i++) {
        auto& currentLayer = layers[i];
        auto& nextLayer = layers[i + 1];

        // Cada nó da camada atual conecta a 1-2 nós da próxima (reduzido para menos densidade)
        for (MapNode* node : currentLayer) {
            // Reduzir probabilidade de múltiplas conexões para tornar o mapa menos denso
            int numConnections = config.minPathsPerNode;
            if (config.maxPathsPerNode > config.minPathsPerNode && Random::GetFloat() < 0.3f) {
                // Apenas 30% de chance de ter mais de uma conexão
                numConnections = config.maxPathsPerNode;
            }
            numConnections = Math::Min(numConnections, (int)nextLayer.size());

            // Escolher aleatoriamente quais nós conectar
            std::vector<int> indices;
            for (size_t j = 0; j < nextLayer.size(); j++) {
                indices.push_back((int)j);
            }

            // Embaralhar
            for (int j = (int)indices.size() - 1; j > 0; j--) {
                int k = Random::GetIntRange(0, j);
                std::swap(indices[j], indices[k]);
            }

            // Conectar
            for (int j = 0; j < numConnections; j++) {
                MapNode* child = nextLayer[indices[j]];
                node->AddChild(child);
                child->AddParent(node);
            }
        }
    }
}

void MapGenerator::EnsureConnectivity(std::vector<std::vector<MapNode*>>& layers) {
    // Garantir que todo nó tem pelo menos um pai (exceto START)
    for (size_t i = 1; i < layers.size(); i++) {
        for (MapNode* node : layers[i]) {
            if (node->GetParents().empty()) {
                // Conectar com um nó aleatório da camada anterior
                int randomIdx = Random::GetIntRange(0, (int)layers[i-1].size() - 1);
                MapNode* parent = layers[i-1][randomIdx];
                parent->AddChild(node);
                node->AddParent(parent);
            }
        }
    }
}

void MapGenerator::CalculateNodePositions(std::vector<MapNode*>& nodes, int screenWidth, int screenHeight) {
    // Agrupar nós por camada
    std::map<int, std::vector<MapNode*>> nodesByLayer;
    int maxLayer = 0;

    for (MapNode* node : nodes) {
        nodesByLayer[node->GetLayer()].push_back(node);
        maxLayer = Math::Max(maxLayer, node->GetLayer());
    }

    // Calcular posições com mais espaçamento
    float marginX = 50.0f;   // Margem esquerda menor para nó inicial mais à esquerda
    float marginY = 40.0f;   // Margem vertical reduzida para usar mais espaço vertical
    float usableWidth = (screenWidth * 2.0f) - 2 * marginX;  // Mapa mais largo
    float usableHeight = screenHeight - 2 * marginY;  // Mais altura utilizável

    for (auto& [layer, layerNodes] : nodesByLayer) {
        // X baseado na camada (esquerda -> direita) com mais espaçamento
        float x = marginX + (usableWidth / Math::Max(1, maxLayer)) * layer;

        // Y distribuído uniformemente entre os nós da camada com mais espaçamento
        float spacing = usableHeight / ((float)layerNodes.size() + 1);

        for (size_t i = 0; i < layerNodes.size(); i++) {
            float y = marginY + spacing * (i + 1);
            layerNodes[i]->SetPosition(Vector2(x, y));
        }
    }
}

