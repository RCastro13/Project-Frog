// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include <algorithm>
#include <vector>
#include <SDL_ttf.h>

#include "Game.h"
#include "Random.h"

#include "Renderer/Renderer.h"
#include "Renderer/Font.h"
#include "Audio/AudioSystem.h"

#include "GameScene/GameScene.h"
#include "GameScene/MainMenuScene.h"

#include "Actors/Actor.h"
#include "Actors/Player.h"
#include "Components/Drawing/DrawComponent.h"
#include "Combat/Card.h"
#include "Map/MapNode.h"

Game::Game()
        :mWindow(nullptr)
        ,mRenderer(nullptr)
        ,mDefaultFont(nullptr)
        ,mAudio(nullptr)
        ,mTicksCount(0)
        ,mIsRunning(true)
        ,mIsDebugging(true)
        ,mUpdatingActors(false)
        ,mCameraPos(Vector2::Zero)
        ,mPlayer(nullptr)
        ,mLevelData(nullptr)
        ,mIsPaused(false)
        ,mCurrentScene(nullptr)
        ,mPendingScene(nullptr)
        ,mCurrentMapNode(nullptr)
        ,mIsChangingScene(false)
        ,mIsFading(false)
        ,mFadeOut(false)
        ,mFadeDuration(0.8f)
        ,mFadeTimer(0.0f)
        ,mFadeAlpha(0.0f)
{

}

bool Game::Initialize()
{
    Random::Init();

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("ERROR: Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow("Project Frog", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    if (!mWindow)
    {
        SDL_Log("ERROR: Failed to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer = new Renderer(mWindow);
    if (!mRenderer->Initialize(WINDOW_WIDTH, WINDOW_HEIGHT))
    {
        SDL_Log("ERROR:Failed to initialize renderer");
        return false;
    }

    if (TTF_Init() != 0)
    {
        SDL_Log("ERROR: Failed to initialize SDL_ttf: %s", TTF_GetError());
        return false;
    }

    mDefaultFont = new Font();
    if (!mDefaultFont->Load("../Assets/Fonts/PressStart2P.ttf"))
    {
        SDL_Log("ERROR: Failed to load default font");
    }

    mAudio = new AudioSystem(8);
    mAudio->CacheAllSounds();

    InitializeActors();

    mCurrentScene = new MainMenuScene(this);
    mCurrentScene->Enter();

    StartFade(false, mFadeDuration);

    mTicksCount = SDL_GetTicks();

    return true;
}

void Game::InitializeActors()
{
    if (!mPlayer) {
        delete mPlayer;
    }

    std::vector<Card*> startDeck = CreateStarterDeck();
    mPlayer = new Player(this, "Frog Hero", 20, 20, startDeck);

    for (Card* card : mPlayer->GetDeck()) {
        card->SetOwner(mPlayer);
    }

}

std::vector<Card*> Game::CreateStarterDeck()
{
    std::vector<Card*> deck;
    deck.push_back(new Card("Fire Strike", AttackType::Fire, 5, 3, nullptr));
    deck.push_back(new Card("Water Shield", AttackType::Water, 4, 2, nullptr));
    deck.push_back(new Card("Plant Whip", AttackType::Plant, 5, 3, nullptr));
    deck.push_back(new Card("Neutral Punch", AttackType::Neutral, 4, 1, nullptr));
    return deck;
}

int **Game::LoadLevel(const std::string& fileName, int width, int height)
{
    return nullptr;
}

void Game::BuildLevel(int** levelData, int width, int height)
{
}

void Game::RunLoop()
{
    while (mIsRunning)
    {
        float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
        if (deltaTime > 0.05f)
        {
            deltaTime = 0.05f;
        }

        mTicksCount = SDL_GetTicks();

        ProcessInput();
        UpdateGame(deltaTime);
        GenerateOutput();

        int sleepTime = (1000 / FPS) - (SDL_GetTicks() - mTicksCount);
        if (sleepTime > 0)
        {
            SDL_Delay(sleepTime);
        }
    }
}

void Game::ProcessInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                Quit();
                break;
        }
    }

    const Uint8* state = SDL_GetKeyboardState(nullptr);

    if (!mIsPaused) {
        if (mCurrentScene)
        {
            mCurrentScene->ProcessInput(state);
        }

        for (auto actor : mActors)
        {
            actor->ProcessInput(state);
        }
    }
}

void Game::UpdateGame(float deltaTime)
{
    UpdateFade(deltaTime);

    if (mCurrentScene && !mIsPaused)
    {
        mCurrentScene->Update(deltaTime);
    }

    if (!mIsPaused) {
        UpdateActors(deltaTime);
    }

    if (mAudio)
    {
        mAudio->Update(deltaTime);
    }

    UpdateCamera();
}

void Game::SetScene(GameScene* scene)
{
    if (mPendingScene)
    {
        SDL_Log("WARNING: Tentou trocar cena enquanto já há uma transição pendente!");
        delete scene;
        return;
    }

    mPendingScene = scene;

    if (!mIsFading)
    {
        StartFade(true);
    }
}

void Game::StartFade(bool fadeOut, float duration)
{
    mIsFading = true;
    mFadeOut = fadeOut;
    mFadeDuration = duration;
    mFadeTimer = 0.0f;
    mFadeAlpha = fadeOut ? 0.0f : 1.0f;
}

void Game::UpdateFade(float deltaTime)
{
    if (!mIsFading) return;

    mFadeTimer += deltaTime;
    float t = Math::Min(mFadeTimer / mFadeDuration, 1.0f);

    if (mFadeOut)
    {
        UpdateFadeOut(t);
    }
    else
    {
        UpdateFadeIn(t);
    }
}

void Game::UpdateFadeOut(float t)
{
    mFadeAlpha = t;

    if (t >= 1.0f)
    {
        mFadeOut = false;
        mFadeTimer = 0.0f;

        if (mPendingScene)
        {
            PerformSceneTransition();
            StartFade(false, mFadeDuration);
        }
        else
        {
            mIsFading = false;
            mFadeAlpha = 1.0f;
        }
    }
}

void Game::UpdateFadeIn(float t)
{
    mFadeAlpha = 1.0f - t;

    if (t >= 1.0f)
    {
        mIsFading = false;
        mFadeAlpha = 0.0f;
    }
}

void Game::PerformSceneTransition()
{
    CleanupCurrentScene();

    mIsChangingScene = true;
    std::vector<Actor*> actorsToDelete = mActors;
    mActors.clear();
    mPendingActors.clear();

    for (auto actor : actorsToDelete)
    {
        if (actor != mPlayer) {
            delete actor;
        }
    }

    if (mPlayer) {
        mActors.push_back(mPlayer);
    }

    mIsChangingScene = false;

    mCurrentScene = mPendingScene;
    mCurrentScene->Enter();
    mPendingScene = nullptr;
}

void Game::CleanupCurrentScene()
{
    if (mCurrentScene)
    {
        mCurrentScene->Exit();
        delete mCurrentScene;
        mCurrentScene = nullptr;
    }
}

void Game::UpdateActors(float deltaTime)
{
    mUpdatingActors = true;
    for (auto actor : mActors)
    {
        actor->Update(deltaTime);
    }
    mUpdatingActors = false;

    for (auto pending : mPendingActors)
    {
        mActors.emplace_back(pending);
    }
    mPendingActors.clear();

    DeleteActorsMarkedForDestroy();
}

void Game::DeleteActorsMarkedForDestroy()
{
    std::vector<Actor*> actorsToDelete;
    for (auto actor : mActors)
    {
        if (actor->GetState() == ActorState::Destroy)
        {
            actorsToDelete.push_back(actor);
        }
    }

    for (auto actor : actorsToDelete)
    {
        auto it = std::find(mActors.begin(), mActors.end(), actor);
        if (it != mActors.end())
        {
            mActors.erase(it);
        }
    }

    for (auto actor : actorsToDelete)
    {
        delete actor;
    }
}

void Game::UpdateCamera()
{
}

void Game::AddActor(Actor* actor)
{
    if (mUpdatingActors)
    {
        mPendingActors.emplace_back(actor);
    }
    else
    {
        mActors.emplace_back(actor);
    }
}

void Game::RemoveActor(Actor* actor)
{
    if (mIsChangingScene)
    {
        return;
    }

    auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
    if (iter != mPendingActors.end())
    {
        std::iter_swap(iter, mPendingActors.end() - 1);
        mPendingActors.pop_back();
    }

    iter = std::find(mActors.begin(), mActors.end(), actor);
    if (iter != mActors.end())
    {
        std::iter_swap(iter, mActors.end() - 1);
        mActors.pop_back();
    }
}

void Game::AddDrawable(class DrawComponent *drawable)
{
    mDrawables.emplace_back(drawable);

    std::sort(mDrawables.begin(), mDrawables.end(),[](DrawComponent* a, DrawComponent* b) {
        return a->GetDrawOrder() < b->GetDrawOrder();
    });
}

void Game::RemoveDrawable(class DrawComponent *drawable)
{
    auto iter = std::find(mDrawables.begin(), mDrawables.end(), drawable);
    mDrawables.erase(iter);
}

void Game::AddCollider(class AABBColliderComponent* collider)
{
    mColliders.emplace_back(collider);
}

void Game::RemoveCollider(AABBColliderComponent* collider)
{
    auto iter = std::find(mColliders.begin(), mColliders.end(), collider);
    mColliders.erase(iter);
}

void Game::GenerateOutput()
{
    mRenderer->Clear();

    if (mCurrentScene)
    {
        mCurrentScene->RenderBackground();
    }

    for (auto drawable : mDrawables)
    {
        drawable->Draw(mRenderer);

        if(mIsDebugging)
        {
              for (auto comp : drawable->GetOwner()->GetComponents())
              {
                comp->DebugDraw(mRenderer);
              }
        }
    }

    if (mCurrentScene)
    {
        mCurrentScene->Render();
    }

    RenderFade();

    mRenderer->Present();
}

void Game::RenderFade()
{
    if (mIsFading && mFadeAlpha > 0.0f)
    {
        mRenderer->DrawRectAlpha(
            Vector2(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f),
            Vector2(static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT)),
            0.0f,
            Vector3(0.0f, 0.0f, 0.0f),
            mFadeAlpha,
            Vector2::Zero,
            RendererMode::TRIANGLES
        );
    }
}

void Game::InitializeMap()
{
    ClearMap();
}

void Game::ClearMap()
{
    for (MapNode* node : mMapNodes) {
        delete node;
    }
    mMapNodes.clear();
    mCurrentMapNode = nullptr;
}

void Game::Shutdown()
{
    ClearMap();
    ShutdownActors();
    ShutdownScenes();
    ShutdownSystems();
}

void Game::ShutdownActors()
{
    while (!mActors.empty()) {
        Actor* actor = mActors.back();
        mActors.pop_back();
        delete actor;
    }
    mPendingActors.clear();

    if (mPlayer) {
        mPlayer = nullptr;
    }
}

void Game::ShutdownScenes()
{
    if (mCurrentScene)
    {
        delete mCurrentScene;
        mCurrentScene = nullptr;
    }
    if (mPendingScene)
    {
        delete mPendingScene;
        mPendingScene = nullptr;
    }
}

void Game::ShutdownSystems()
{
    if (mLevelData) {
        for (int i = 0; i < LEVEL_HEIGHT; ++i) {
            delete[] mLevelData[i];
        }
        delete[] mLevelData;
        mLevelData = nullptr;
    }

    if (mAudio)
    {
        delete mAudio;
        mAudio = nullptr;
    }

    if (mDefaultFont)
    {
        mDefaultFont->Unload();
        delete mDefaultFont;
        mDefaultFont = nullptr;
    }

    TTF_Quit();

    mRenderer->Shutdown();
    delete mRenderer;
    mRenderer = nullptr;

    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}