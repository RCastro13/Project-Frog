#pragma once
#include "GameScene.h"
#include "../Map/MapNode.h"
#include "../Map/MapNodeType.h"
#include "../Math.h"
#include "../Audio/AudioSystem.h"
#include <vector>
#include <map>
#include <string>

class Texture;

class MapScene : public GameScene
{
public:
    MapScene(class Game* game);
    ~MapScene();

    void Enter() override;
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* keyState) override;
    void RenderBackground() override;
    void Exit() override;

    SceneType GetType() const override { return SceneType::MAP; }
    const char* GetName() const override { return "Map"; }

private:
    void RenderMap();
    void RenderNode(MapNode* node);
    void RenderConnections();
    void SelectNextAccessibleNode();
    void SelectPreviousAccessibleNode();
    void ConfirmSelection();
    bool CanSelectNode(MapNode* node);
    std::vector<MapNode*> GetAccessibleNodes();
    void LoadAvailableIcons();
    std::string GetIconPathForNodeType(MapNodeType type);
    void AssignIconsToNodes();
    const char* GetNodeTypeName(MapNodeType type);
    void SetCurrentNode(MapNode* node);

    std::vector<MapNode*> mMapNodes;
    std::map<MapNode*, Texture*> mNodeIcons;
    std::vector<std::string> mAvailableIcons;
    MapNode* mCurrentNode;
    MapNode* mSelectedNode;
    int mSelectedIndex;
    bool mConfirming;
    class Texture* mBackgroundTexture;
    Vector2 mCameraPosition;
    float mMinCameraX;
    float mMaxCameraX;
    SoundHandle mMapSound;
};

