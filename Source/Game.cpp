// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include <algorithm>
#include <vector>
#include <map>
#include <fstream>
#include "CSV.h"
#include "Game.h"
#include "Components/Drawing/DrawComponent.h"
#include "Components/Physics/RigidBodyComponent.h"
#include "Random.h"
#include "Actors/Actor.h"

#include <sstream>
#include <SDL_ttf.h>

#include "Renderer/Shader.h"
#include "GameScene/GameScene.h"
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
        ,mMario(nullptr)
        ,mLevelData(nullptr)
        ,mIsPaused(false)
        ,mCurrentScene(nullptr)
        ,mPendingScene(nullptr)
        ,mCurrentMapNode(nullptr)
{

}

bool Game::Initialize()
{
    Random::Init();

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow("Project Frog", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    if (!mWindow)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer = new Renderer(mWindow);
    if (!mRenderer->Initialize(WINDOW_WIDTH, WINDOW_HEIGHT))
    {
        SDL_Log("Failed to initialize renderer");
        return false;
    }

    // Inicializar SDL_ttf
    if (TTF_Init() != 0)
    {
        SDL_Log("Failed to initialize SDL_ttf: %s", TTF_GetError());
        return false;
    }

    // Carregar fonte padrão
    mDefaultFont = new Font();
    if (!mDefaultFont->Load("../Assets/Fonts/PressStart2P.ttf"))
    {
        SDL_Log("Failed to load default font");
        // Não retornar false - continuar sem fonte
    }

    // Inicializar sistema de áudio
    mAudio = new AudioSystem(8);
    SDL_Log("Audio system initialized");
    // CacheAllSounds() será chamado quando houver sons em Assets/Sounds/

    // Init all game actors
    InitializeActors();

    // Iniciar com a cena do menu principal
    mCurrentScene = new MainMenuScene(this);
    mCurrentScene->Enter();

    mTicksCount = SDL_GetTicks();

    return true;
}

void Game::InitializeActors()
{
}

int **Game::LoadLevel(const std::string& fileName, int width, int height)
{
    // TODO: Implementar carregamento de nível
    return nullptr;
}

void Game::BuildLevel(int** levelData, int width, int height)
{
}

void Game::RunLoop()
{
    while (mIsRunning)
    {
        // Calculate delta time in seconds
        float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
        if (deltaTime > 0.05f)
        {
            deltaTime = 0.05f;
        }

        mTicksCount = SDL_GetTicks();

        ProcessInput();
        UpdateGame(deltaTime);
        GenerateOutput();

        // Sleep to maintain frame rate
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
        // Process input da cena atual
        if (mCurrentScene)
        {
            mCurrentScene->ProcessInput(state);
        }

        // Process input dos atores (mantido para compatibilidade)
        for (auto actor : mActors)
        {
            actor->ProcessInput(state);
        }
    }
}

void Game::UpdateGame(float deltaTime)
{
    // Troca de cena pendente
    if (mPendingScene)
    {
        if (mCurrentScene)
        {
            mCurrentScene->Exit();
            delete mCurrentScene;
        }
        mCurrentScene = mPendingScene;
        mCurrentScene->Enter();
        mPendingScene = nullptr;
    }

    // Update da cena atual
    if (mCurrentScene && !mIsPaused)
    {
        mCurrentScene->Update(deltaTime);
    }

    // Update all actors and pending actors
    if (!mIsPaused) {
        UpdateActors(deltaTime);
    }

    // Update do sistema de áudio
    if (mAudio)
    {
        mAudio->Update(deltaTime);
    }

    // Update camera position
    UpdateCamera();
}

void Game::SetScene(GameScene* scene)
{
    // Não trocar imediatamente para evitar problemas
    // A troca será feita no início do próximo frame
    mPendingScene = scene;
    SDL_Log("Scene change requested to: %s", scene->GetName());
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

    std::vector<Actor*> deadActors;
    for (auto actor : mActors)
    {
        if (actor->GetState() == ActorState::Destroy)
        {
            deadActors.emplace_back(actor);
        }
    }

    for (auto actor : deadActors)
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
    auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
    if (iter != mPendingActors.end())
    {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, mPendingActors.end() - 1);
        mPendingActors.pop_back();
    }

    iter = std::find(mActors.begin(), mActors.end(), actor);
    if (iter != mActors.end())
    {
        // Swap to end of vector and pop off (avoid erase copies)
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
    // Clear back buffer
    mRenderer->Clear();

    // Render scene background (ANTES dos actors)
    if (mCurrentScene)
    {
        mCurrentScene->RenderBackground();
    }

    for (auto drawable : mDrawables)
    {
        drawable->Draw(mRenderer);

        if(mIsDebugging)
        {
           // Call draw for actor components
              for (auto comp : drawable->GetOwner()->GetComponents())
              {
                comp->DebugDraw(mRenderer);
              }
        }
    }

    // Render scene UI (DEPOIS dos actors)
    if (mCurrentScene)
    {
        mCurrentScene->Render();
    }

    // Swap front buffer and back buffer
    mRenderer->Present();
}

void Game::InitializeMap()
{
    // Este método pode ser chamado para forçar geração de um novo mapa
    // Por padrão, o mapa é gerado quando a MapScene é criada pela primeira vez
    ClearMap();
}

void Game::ClearMap()
{
    // Limpar nós do mapa existente
    for (MapNode* node : mMapNodes) {
        delete node;
    }
    mMapNodes.clear();
    mCurrentMapNode = nullptr;
}

void Game::Shutdown()
{
    // Limpar mapa
    ClearMap();


    // Limpar cenas
    if (mCurrentScene)
    {
        mCurrentScene->Exit();
        delete mCurrentScene;
        mCurrentScene = nullptr;
    }
    if (mPendingScene)
    {
        delete mPendingScene;
        mPendingScene = nullptr;
    }

    // Limpar atores
    while (!mActors.empty()) {
        delete mActors.back();
    }

    // Delete level data
    if (mLevelData) {
        for (int i = 0; i < LEVEL_HEIGHT; ++i) {
            delete[] mLevelData[i];
        }
        delete[] mLevelData;
        mLevelData = nullptr;
    }

    // Desligar sistema de áudio
    if (mAudio)
    {
        delete mAudio;
        mAudio = nullptr;
    }

    // Limpar fonte
    if (mDefaultFont)
    {
        mDefaultFont->Unload();
        delete mDefaultFont;
        mDefaultFont = nullptr;
    }

    // Finalizar SDL_ttf
    TTF_Quit();

    mRenderer->Shutdown();
    delete mRenderer;
    mRenderer = nullptr;

    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}