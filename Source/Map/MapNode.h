#pragma once
#include "MapNodeType.h"
#include "../Math.h"
#include <vector>

class MapNode {
public:
    MapNode(int id, MapNodeType type, int layer);
    ~MapNode();

    // Getters
    int GetID() const { return mID; }
    MapNodeType GetType() const { return mType; }
    int GetLayer() const { return mLayer; }
    Vector2 GetPosition() const { return mPosition; }
    bool IsCompleted() const { return mCompleted; }
    bool IsAccessible() const { return mAccessible; }
    bool IsCurrent() const { return mIsCurrent; }

    // Setters
    void SetPosition(Vector2 pos) { mPosition = pos; }
    void SetCompleted(bool completed) { mCompleted = completed; }
    void SetAccessible(bool accessible) { mAccessible = accessible; }
    void SetCurrent(bool current) { mIsCurrent = current; }

    // Conexões
    void AddChild(MapNode* child);
    void AddParent(MapNode* parent);
    const std::vector<MapNode*>& GetChildren() const { return mChildren; }
    const std::vector<MapNode*>& GetParents() const { return mParents; }

private:
    int mID;
    MapNodeType mType;
    int mLayer;                      // Profundidade na árvore (0 = START, N = BOSS)
    Vector2 mPosition;               // Posição visual na tela
    bool mCompleted;                 // Jogador já passou por aqui?
    bool mAccessible;                // Jogador pode acessar este nó?
    bool mIsCurrent;                 // Jogador está neste nó?
    std::vector<MapNode*> mChildren; // Próximos nós possíveis
    std::vector<MapNode*> mParents;  // Nós que levam a este
};

