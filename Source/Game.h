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

class Game
{
public:
    Game();

    bool Initialize();
    void RunLoop();
    void Shutdown();
    void Quit() { mIsRunning = false; }

    // Actor functions
    void InitializeActors();
    void UpdateActors(float deltaTime);
    void AddActor(class Actor* actor);
    void RemoveActor(class Actor* actor);

    // Game scenes
    void SetScene(GameScene* scene);
    GameScene* GetCurrentScene() const { return mCurrentScene; }

    // Fade system
    void StartFade(bool fadeOut, float duration = 0.8f);
    bool IsFading() const { return mIsFading; }

    // Map management
    void InitializeMap();
    void ClearMap();
    std::vector<class MapNode*> GetMapNodes() const { return mMapNodes; }
    void SetMapNodes(const std::vector<class MapNode*>& nodes) { mMapNodes = nodes; }
    class MapNode* GetCurrentMapNode() const { return mCurrentMapNode; }
    void SetCurrentMapNode(class MapNode* node) { mCurrentMapNode = node; }

    // Renderer
    class Renderer* GetRenderer() { return mRenderer; }

    // Font and Audio
    class Font* GetFont() { return mDefaultFont; }
    class AudioSystem* GetAudio() { return mAudio; }

    // Window
    SDL_Window* GetWindow() { return mWindow; }

    static const int WINDOW_WIDTH   = 640;
    static const int WINDOW_HEIGHT  = 448;
    static const int LEVEL_WIDTH    = 215;
    static const int LEVEL_HEIGHT   = 15;
    static const int TILE_SIZE      = 32;
    static const int SPAWN_DISTANCE = 700;
    static const int FPS = 60;

    // Draw functions
    void AddDrawable(class DrawComponent* drawable);
    void RemoveDrawable(class DrawComponent* drawable);
    std::vector<class DrawComponent*>& GetDrawables() { return mDrawables; }

    // Collider functions
    void AddCollider(class AABBColliderComponent* collider);
    void RemoveCollider(class AABBColliderComponent* collider);
    std::vector<class AABBColliderComponent*>& GetColliders() { return mColliders; }

    // Camera functions
    Vector2& GetCameraPos() { return mCameraPos; };
    void SetCameraPos(const Vector2& position) { mCameraPos = position; };

    // Game specific
    Player* GetPlayer() { return mPlayer; }

private:
    void ProcessInput();
    void UpdateGame(float deltaTime);
    void UpdateCamera();
    void GenerateOutput();
    void UpdateFade(float deltaTime);
    void RenderFade();

    // Level loading
    int **LoadLevel(const std::string& fileName, int width, int height);
    void BuildLevel(int** levelData, int width, int height);

    // All the actors in the game
    std::vector<class Actor*> mActors;
    std::vector<class Actor*> mPendingActors;

    // Flag para indicar que estamos em transição de cena
    bool mIsChangingScene;

    // Camera
    Vector2 mCameraPos;

    // All the draw components
    std::vector<class DrawComponent*> mDrawables;

    // All the collision components
    std::vector<class AABBColliderComponent*> mColliders;

    // SDL stuff
    SDL_Window* mWindow;
    class Renderer* mRenderer;

    // Font and Audio systems
    class Font* mDefaultFont;
    class AudioSystem* mAudio;

    // Track elapsed time since game start
    Uint32 mTicksCount;

    // Track if we're updating actors right now
    bool mIsRunning;
    bool mIsDebugging;
    bool mUpdatingActors;
    bool mIsPaused;

    // Scenes & state
    GameScene* mCurrentScene;
    GameScene* mPendingScene;  // Para trocar cena no próximo frame

    // Fade system
    bool mIsFading;
    bool mFadeOut;
    float mFadeDuration;
    float mFadeTimer;
    float mFadeAlpha;

    // Map system
    std::vector<class MapNode*> mMapNodes;
    class MapNode* mCurrentMapNode;

    // Game-specific
    Player* mPlayer;
    int **mLevelData;
};
