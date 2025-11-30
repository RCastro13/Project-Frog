// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <SDL.h>
#include <vector>

#include "GameScene/GameScene.h"
#include "Renderer/Renderer.h"
#include "Renderer/Font.h"
#include "Audio/AudioSystem.h"

class Player;
class MapNode;
class Card;

class Game
{
public:
    Game();

    bool Initialize();
    void RunLoop();
    void Shutdown();
    void Quit() { mIsRunning = false; }

    void InitializeActors();
    void UpdateActors(float deltaTime);
    void AddActor(class Actor* actor);
    void RemoveActor(class Actor* actor);

    void SetScene(GameScene* scene);
    GameScene* GetCurrentScene() const { return mCurrentScene; }

    void StartFade(bool fadeOut, float duration = 0.8f);
    bool IsFading() const { return mIsFading; }

    void InitializeMap();
    void ClearMap();
    std::vector<class MapNode*> GetMapNodes() const { return mMapNodes; }
    void SetMapNodes(const std::vector<class MapNode*>& nodes) { mMapNodes = nodes; }
    class MapNode* GetCurrentMapNode() const { return mCurrentMapNode; }
    void SetCurrentMapNode(class MapNode* node) { mCurrentMapNode = node; }

    class Renderer* GetRenderer() { return mRenderer; }

    class Font* GetFont() { return mDefaultFont; }
    class AudioSystem* GetAudio() { return mAudio; }

    SDL_Window* GetWindow() { return mWindow; }

    static const int WINDOW_WIDTH   = 640;
    static const int WINDOW_HEIGHT  = 448;
    static const int LEVEL_WIDTH    = 215;
    static const int LEVEL_HEIGHT   = 15;
    static const int TILE_SIZE      = 32;
    static const int SPAWN_DISTANCE = 700;
    static const int FPS = 60;

    void AddDrawable(class DrawComponent* drawable);
    void RemoveDrawable(class DrawComponent* drawable);
    std::vector<class DrawComponent*>& GetDrawables() { return mDrawables; }

    void AddCollider(class AABBColliderComponent* collider);
    void RemoveCollider(class AABBColliderComponent* collider);
    std::vector<class AABBColliderComponent*>& GetColliders() { return mColliders; }

    Vector2& GetCameraPos() { return mCameraPos; };
    void SetCameraPos(const Vector2& position) { mCameraPos = position; };

    Player* GetPlayer() { return mPlayer; }

private:
    void ProcessInput();
    void UpdateGame(float deltaTime);
    void UpdateCamera();
    void GenerateOutput();
    void UpdateFade(float deltaTime);
    void UpdateFadeOut(float deltaTime);
    void UpdateFadeIn(float deltaTime);
    void RenderFade();

    void DeleteActorsMarkedForDestroy();
    void PerformSceneTransition();
    void CleanupCurrentScene();

    void ShutdownActors();
    void ShutdownScenes();
    void ShutdownSystems();

    std::vector<Card*> CreateStarterDeck();

    int **LoadLevel(const std::string& fileName, int width, int height);
    void BuildLevel(int** levelData, int width, int height);

    std::vector<class Actor*> mActors;
    std::vector<class Actor*> mPendingActors;

    bool mIsChangingScene;

    Vector2 mCameraPos;

    std::vector<class DrawComponent*> mDrawables;
    std::vector<class AABBColliderComponent*> mColliders;

    SDL_Window* mWindow;
    class Renderer* mRenderer;

    class Font* mDefaultFont;
    class AudioSystem* mAudio;

    Uint32 mTicksCount;

    bool mIsRunning;
    bool mIsDebugging;
    bool mUpdatingActors;
    bool mIsPaused;

    GameScene* mCurrentScene;
    GameScene* mPendingScene;

    bool mIsFading;
    bool mFadeOut;
    float mFadeDuration;
    float mFadeTimer;
    float mFadeAlpha;

    std::vector<class MapNode*> mMapNodes;
    class MapNode* mCurrentMapNode;

    Player* mPlayer;
    int **mLevelData;
};
