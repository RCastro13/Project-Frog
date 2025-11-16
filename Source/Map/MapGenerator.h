#pragma once
#include "MapNode.h"
#include <vector>
#include <map>

struct MapConfig {
    int minLayers = 8;           // Número mínimo de camadas
    int maxLayers = 10;          // Número máximo de camadas
    int minNodesPerLayer = 3;    // Mínimo de nós por camada
    int maxNodesPerLayer = 4;    // Máximo de nós por camada
    int minPathsPerNode = 1;     // Mínimo de caminhos saindo de cada nó
    int maxPathsPerNode = 1;     // Máximo de caminhos saindo de cada nó (reduzido para menos densidade)

    // Probabilidades de cada tipo (soma deve ser ~1.0)
    float combatChance = 0.50f;
    float eliteChance = 0.10f;
    float shopChance = 0.15f;
    float treasureChance = 0.15f;
    float restChance = 0.10f;
};

class MapGenerator {
public:
    static std::vector<MapNode*> Generate(const MapConfig& config = MapConfig());

private:
    static MapNodeType SelectRandomType(int layer, int totalLayers, const MapConfig& config);
    static void ConnectLayers(std::vector<std::vector<MapNode*>>& layers, const MapConfig& config);
    static void CalculateNodePositions(std::vector<MapNode*>& nodes, int screenWidth, int screenHeight);
    static void EnsureConnectivity(std::vector<std::vector<MapNode*>>& layers);
};

