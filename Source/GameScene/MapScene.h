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

    static void ResetMapIntro();

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

    // Estados para a animação da câmera
    enum class SceneState {
        INTRO_PAN_RIGHT, // Indo para a direita
        INTRO_PAN_LEFT,  // Voltando para a esquerda
        INTRO_WAIT,      // Pequena espera
        PLAYING          // Jogador controla
    };

    SceneState mSceneState;
    float mIntroSpeed; // Velocidade da câmera automática
    float mWaitTimer;

    // Variável estática para saber se já mostramos a intro nessa execução do jogo
    static bool s_HasShownMapIntro;

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

