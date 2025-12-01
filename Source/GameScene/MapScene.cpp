#include "MapScene.h"
#include "CombatScene.h"
#include "RewardScene.h"
#include "ShopScene.h"
#include "RestScene.h"
#include "../Game.h"
#include "../Map/MapGenerator.h"
#include "../Random.h"
#include "../Renderer/Texture.h"
#include "../Renderer/Renderer.h"
#include <SDL.h>
#include <SDL_log.h>

MapScene::MapScene(Game* game)
    : GameScene(game)
    , mCurrentNode(nullptr)
    , mSelectedNode(nullptr)
    , mSelectedIndex(0)
    , mConfirming(false)
    , mBackgroundTexture(nullptr)
    , mCameraPosition(Vector2::Zero)
    , mMinCameraX(-200.0f)
    , mMaxCameraX( Game::WINDOW_WIDTH + 200.0f)
{
}

MapScene::~MapScene()
{
    mMapNodes.clear();
    mNodeIcons.clear();
    mAvailableIcons.clear();
}

void MapScene::Enter()
{
    mStateTime = 0.0f;
    mConfirming = false;

    SDL_SetWindowTitle(mGame->GetWindow(), "Project Frog");

    mBackgroundTexture = mGame->GetRenderer()->GetTexture("../Assets/Background/Map/mapa.png");

    mGame->GetRenderer()->SetClearColor(0.2f, 0.5f, 0.3f, 1.0f);

    mMapSound = mGame->GetAudio()->PlaySound("MapSound.wav", true);

    if (mGame->GetMapNodes().empty()) {
        mMapNodes = MapGenerator::Generate();
        mGame->SetMapNodes(mMapNodes);

        if (!mMapNodes.empty()) {
            mCurrentNode = mMapNodes[0];
            mGame->SetCurrentMapNode(mCurrentNode);
        }
    } else {
        mMapNodes = mGame->GetMapNodes();
        mCurrentNode = mGame->GetCurrentMapNode();
    }

    if (mCurrentNode) {
        for (MapNode* node : mMapNodes) {
            if (node != mCurrentNode) {
                node->SetAccessible(false);
            }
        }
        for (MapNode* child : mCurrentNode->GetChildren()) {
            child->SetAccessible(true);
        }
    }

    std::vector<MapNode*> accessible = GetAccessibleNodes();
    if (!accessible.empty()) {
        mSelectedNode = accessible[0];
        mSelectedIndex = 0;
    }

    LoadAvailableIcons();
    if (mNodeIcons.empty() || mNodeIcons.size() != mMapNodes.size()) {
        AssignIconsToNodes();
    }

    if (mCurrentNode) {
        Vector2 startPos = mCurrentNode->GetPosition();
        mCameraPosition = Vector2(startPos.x - 100.0f, 0.0f);
    }
}

void MapScene::Update(float deltaTime)
{
    mStateTime += deltaTime;

    UpdateFade(deltaTime);

    const Uint8* keyState = SDL_GetKeyboardState(nullptr);
    static const float scrollSpeed = 300.0f;

    if (keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A]) {
        mCameraPosition.x -= scrollSpeed * deltaTime;
    }
    if (keyState[SDL_SCANCODE_RIGHT] || keyState[SDL_SCANCODE_D]) {
        mCameraPosition.x += scrollSpeed * deltaTime;
    }

    if (mCameraPosition.x < mMinCameraX) {
        mCameraPosition.x = mMinCameraX;
    }
    if (mCameraPosition.x > mMaxCameraX) {
        mCameraPosition.x = mMaxCameraX;
    }
}

void MapScene::ProcessInput(const Uint8* keyState)
{
    if (mFadeAlpha > 0.0f || mGame->IsFading())
        return;

    static bool upWasPressed = false;
    static bool downWasPressed = false;
    static bool enterWasPressed = false;

    if (keyState[SDL_SCANCODE_UP] && !upWasPressed) {
        MapNode* previousSelection = mSelectedNode;
        SelectPreviousAccessibleNode();

        if (mSelectedNode && mSelectedNode != previousSelection) {
            int randomChoiceAudio = Random::GetIntRange(1, 4);
            std::string audio = "MapChoice" + std::to_string(randomChoiceAudio) + ".ogg";
            mGame->GetAudio()->PlaySound(audio, false);
        }
        upWasPressed = true;
    } else if (!keyState[SDL_SCANCODE_UP]) {
        upWasPressed = false;
    }

    if (keyState[SDL_SCANCODE_DOWN] && !downWasPressed) {
        MapNode* previousSelection = mSelectedNode;
        SelectNextAccessibleNode();

        if (mSelectedNode && mSelectedNode != previousSelection) {
            int randomChoiceAudio = Random::GetIntRange(1, 4);
            std::string audio = "MapChoice" + std::to_string(randomChoiceAudio) + ".ogg";
            mGame->GetAudio()->PlaySound(audio, false);
        }
        downWasPressed = true;
    } else if (!keyState[SDL_SCANCODE_DOWN]) {
        downWasPressed = false;
    }

    if (keyState[SDL_SCANCODE_RETURN] && !enterWasPressed && !mConfirming) {
        MapNodeType nodeType = mSelectedNode ? mSelectedNode->GetType() : MapNodeType::START;

        ConfirmSelection();
        enterWasPressed = true;

        if (nodeType == MapNodeType::BOSS || nodeType == MapNodeType::COMBAT) {
            mGame->GetAudio()->PlaySound("MapEnemySelection.ogg", false);
        } else if (nodeType == MapNodeType::SHOP) {
            mGame->GetAudio()->PlaySound("MapStoreSelection.ogg", false);
        } else if (nodeType == MapNodeType::REST) {
            mGame->GetAudio()->PlaySound("MapHealthSelection.ogg", false);
        } else if (nodeType == MapNodeType::TREASURE) {
            mGame->GetAudio()->PlaySound("MapCoinSelection.ogg", false);
        }
    } else if (!keyState[SDL_SCANCODE_RETURN]) {
        enterWasPressed = false;
    }
}

void MapScene::Exit()
{
    mBackgroundTexture = nullptr;
    mGame->GetAudio()->StopSound(mMapSound);
}

void MapScene::SetCurrentNode(MapNode* node)
{
    if (mCurrentNode) {
        mCurrentNode->SetCurrent(false);
    }
    mCurrentNode = node;
    if (mCurrentNode) {
        mCurrentNode->SetCurrent(true);
    }
}

void MapScene::RenderBackground()
{
    if (mBackgroundTexture) {
        float backgroundWidth = 1280.0f + 400.0f;
        float backgroundHeight = 448.0f;

        float centerX = backgroundWidth / 2.0f - 200.0f;
        float centerY = backgroundHeight / 2.0f;

        mGame->GetRenderer()->DrawTexture(
            Vector2(centerX, centerY),
            Vector2(backgroundWidth, backgroundHeight),
            0.0f,
            Vector3(1.0f, 1.0f, 1.0f),
            mBackgroundTexture,
            Vector4::UnitRect,
            mCameraPosition,
            false,
            1.0f
        );
    }

    RenderMap();
}

void MapScene::RenderMap()
{
    RenderConnections();

    for (MapNode* node : mMapNodes) {
        RenderNode(node);
    }
}

void MapScene::RenderNode(MapNode* node)
{
    Vector2 pos = node->GetPosition();
    float size = 48.0f;

    std::string iconFolder = "White";
    if (node == mCurrentNode) {
        iconFolder = "Green";
    } else if (node->IsCompleted()) {
        iconFolder = "Red";
    } else {
        iconFolder = "White";
    }

    std::string iconName = GetIconPathForNodeType(node->GetType());
    size_t posWhite = iconName.find("/White/");
    if (posWhite != std::string::npos) {
        iconName.replace(posWhite, 7, "/" + iconFolder + "/");
    }

    Texture* iconTexture = mGame->GetRenderer()->GetTexture(iconName);

    if (!iconTexture) {
        std::string fallbackPath = GetIconPathForNodeType(node->GetType());
        iconTexture = mGame->GetRenderer()->GetTexture(fallbackPath);
    }

    Vector3 color = Vector3(1.0f, 1.0f, 1.0f);

    if (node == mSelectedNode && node != mCurrentNode && CanSelectNode(node)) {
        float pulse = 0.5f + 0.3f * Math::Sin(mStateTime * 4.0f);
        float highlightSize = size + 12.0f * pulse;

        mGame->GetRenderer()->DrawRect(
            pos,
            Vector2(highlightSize, highlightSize),
            0.0f,
            Vector3(1.0f, 1.0f, 0.0f),
            mCameraPosition,
            RendererMode::TRIANGLES
        );
    }

    if (iconTexture) {
        mGame->GetRenderer()->DrawTexture(
            pos,
            Vector2(size, size),
            0.0f,
            color,
            iconTexture,
            Vector4::UnitRect,
            mCameraPosition,
            false,
            1.0f
        );
    } else {
        mGame->GetRenderer()->DrawRect(
            pos,
            Vector2(size, size),
            0.0f,
            color,
            mCameraPosition,
            RendererMode::TRIANGLES
        );
    }
}

void MapScene::RenderConnections()
{
    for (MapNode* node : mMapNodes) {
        Vector2 startPos = node->GetPosition();

        for (MapNode* child : node->GetChildren()) {
            Vector2 endPos = child->GetPosition();

            Vector3 lineColor = Vector3(0.3f, 0.3f, 0.3f);
            if (node->IsAccessible() || child->IsAccessible()) {
                lineColor = Vector3(0.6f, 0.6f, 0.6f);
            }
            if (node->IsCompleted()) {
                lineColor = Vector3(0.0f, 0.7f, 0.0f);
            }

            Vector2 diff = endPos - startPos;
            float length = diff.Length();
            float angle = Math::Atan2(diff.y, diff.x);
            Vector2 midpoint = startPos + diff * 0.5f;

            mGame->GetRenderer()->DrawRect(
                midpoint,
                Vector2(length, 2.0f),
                angle,
                lineColor,
                mCameraPosition,
                RendererMode::TRIANGLES
            );
        }
    }
}

void MapScene::SelectNextAccessibleNode()
{
    std::vector<MapNode*> accessible = GetAccessibleNodes();

    if (accessible.empty()) {
        mSelectedNode = nullptr;
        mSelectedIndex = 0;
        return;
    }

    if (!mSelectedNode) {
        mSelectedNode = accessible[0];
        mSelectedIndex = 0;
        return;
    }

    int currentIndex = -1;
    for (size_t i = 0; i < accessible.size(); i++) {
        if (accessible[i] == mSelectedNode) {
            currentIndex = static_cast<int>(i);
            break;
        }
    }

    if (currentIndex == -1) {
        mSelectedNode = accessible[0];
        mSelectedIndex = 0;
        return;
    }

    mSelectedIndex = (currentIndex + 1) % static_cast<int>(accessible.size());
    mSelectedNode = accessible[mSelectedIndex];
}

void MapScene::SelectPreviousAccessibleNode()
{
    std::vector<MapNode*> accessible = GetAccessibleNodes();

    if (accessible.empty()) {
        mSelectedNode = nullptr;
        mSelectedIndex = 0;
        return;
    }

    if (!mSelectedNode) {
        mSelectedNode = accessible.back();
        mSelectedIndex = static_cast<int>(accessible.size()) - 1;
        return;
    }

    int currentIndex = -1;
    for (size_t i = 0; i < accessible.size(); i++) {
        if (accessible[i] == mSelectedNode) {
            currentIndex = static_cast<int>(i);
            break;
        }
    }

    if (currentIndex == -1) {
        mSelectedNode = accessible[0];
        mSelectedIndex = 0;
        return;
    }

    mSelectedIndex = (currentIndex - 1 + static_cast<int>(accessible.size()))
                     % static_cast<int>(accessible.size());
    mSelectedNode = accessible[mSelectedIndex];
}

void MapScene::ConfirmSelection()
{
    if (mConfirming) return;
    mConfirming = true;

    if (!mSelectedNode || !CanSelectNode(mSelectedNode)) {
        return;
    }

    MapNode* nodeToMoveTo = mSelectedNode;

    if (mCurrentNode) {
        mCurrentNode->SetCompleted(true);
        mCurrentNode->SetCurrent(false);
    }

    SetCurrentNode(nodeToMoveTo);
    mGame->SetCurrentMapNode(nodeToMoveTo);

    mSelectedNode = nullptr;
    mSelectedIndex = 0;

    switch (nodeToMoveTo->GetType()) {
        case MapNodeType::START:
        case MapNodeType::COMBAT:
            mGame->SetScene(new CombatScene(mGame));
            break;

        case MapNodeType::ELITE:
            mGame->SetScene(new CombatScene(mGame));
            break;

        case MapNodeType::BOSS:
            mGame->SetScene(new CombatScene(mGame));
            break;

        case MapNodeType::SHOP:
            mGame->SetScene(new ShopScene(mGame));
            break;

        case MapNodeType::TREASURE:
            mGame->SetScene(new RewardScene(mGame, RewardMode::TREASURE_CHEST));
            break;

        case MapNodeType::REST:
            mGame->SetScene(new RestScene(mGame));
            break;
    }
}

bool MapScene::CanSelectNode(MapNode* node)
{
    if (!node) return false;

    if (node->GetType() == MapNodeType::START) return false;

    if (node == mCurrentNode) return false;

    return node->IsAccessible() && !node->IsCompleted();
}

std::vector<MapNode*> MapScene::GetAccessibleNodes()
{
    std::vector<MapNode*> accessible;

    for (MapNode* node : mMapNodes) {
        if (CanSelectNode(node)) {
            accessible.push_back(node);
        }
    }

    return accessible;
}

void MapScene::LoadAvailableIcons()
{
}

std::string MapScene::GetIconPathForNodeType(MapNodeType type)
{
    switch (type) {
        case MapNodeType::START:
            return "../Assets/Icons/White/icon_flag.png";
        case MapNodeType::COMBAT:
            return "../Assets/Icons/White/icon_sword.png";
        case MapNodeType::ELITE:
            return "../Assets/Icons/White/icon_skull.png";
        case MapNodeType::SHOP:
            return "../Assets/Icons/White/icon_coin.png";
        case MapNodeType::TREASURE:
            return "../Assets/Icons/White/icon_chest.png";
        case MapNodeType::REST:
            return "../Assets/Icons/White/icon_heart.png";
        case MapNodeType::BOSS:
            return "../Assets/Icons/White/icon_skull.png";
        default:
            return "../Assets/Icons/White/icon_circle.png";
    }
}

void MapScene::AssignIconsToNodes()
{
    mNodeIcons.clear();

    for (MapNode* node : mMapNodes) {
        std::string iconPath = GetIconPathForNodeType(node->GetType());

        Texture* iconTexture = mGame->GetRenderer()->GetTexture(iconPath);
        if (iconTexture) {
            mNodeIcons[node] = iconTexture;
        } else {
            SDL_Log("ERROR: Failed to load icon: %s", iconPath.c_str());
        }
    }
}

const char* MapScene::GetNodeTypeName(MapNodeType type)
{
    switch (type) {
        case MapNodeType::START:    return "START";
        case MapNodeType::COMBAT:   return "COMBAT";
        case MapNodeType::ELITE:    return "ELITE";
        case MapNodeType::SHOP:     return "SHOP";
        case MapNodeType::TREASURE: return "TREASURE";
        case MapNodeType::REST:     return "REST";
        case MapNodeType::BOSS:     return "BOSS";
        default:                    return "UNKNOWN";
    }
}

