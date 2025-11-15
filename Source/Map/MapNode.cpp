#include "MapNode.h"
#include <algorithm>

MapNode::MapNode(int id, MapNodeType type, int layer)
    : mID(id)
    , mType(type)
    , mLayer(layer)
    , mPosition(Vector2::Zero)
    , mCompleted(false)
    , mAccessible(false)
    , mIsCurrent(false)
{
}

MapNode::~MapNode()
{
    // Não deletar children/parents pois são gerenciados externamente
}

void MapNode::AddChild(MapNode* child)
{
    // Evitar duplicatas
    auto it = std::find(mChildren.begin(), mChildren.end(), child);
    if (it == mChildren.end()) {
        mChildren.push_back(child);
    }
}

void MapNode::AddParent(MapNode* parent)
{
    // Evitar duplicatas
    auto it = std::find(mParents.begin(), mParents.end(), parent);
    if (it == mParents.end()) {
        mParents.push_back(parent);
    }
}

