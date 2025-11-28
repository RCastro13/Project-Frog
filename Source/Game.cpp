// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include <algorithm>
#include <vector>
#include "Game.h"
#include "Components/Drawing/DrawComponent.h"
#include "Random.h"
#include "Actors/Actor.h"

#include <SDL_ttf.h>

#include "GameScene/GameScene.h"
#include "Map/MapNode.h"

#include "Actors/Player.h"
#include "Combat//Card.h"

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
    mAudio->CacheAllSounds(); //será chamado quando houver sons em Assets/Sounds/

    // Init all game actors
    InitializeActors();

    // Iniciar com a cena do menu principal
    mCurrentScene = new MainMenuScene(this);
    mCurrentScene->Enter();

    // Iniciar o jogo com fade in
    StartFade(false, mFadeDuration);

    mTicksCount = SDL_GetTicks();

    return true;
}

void Game::InitializeActors()
{
    if (!mPlayer) {
        delete mPlayer;     // Para cuando se reinicia el juego.
    }

    // criar deck inicial
    std::vector<Card*> startDeck;
    startDeck.push_back(new Card("Fire Strike", AttackType::Fire, 5, 2, nullptr));
    startDeck.push_back(new Card("Water Shield", AttackType::Water, 4, 1, nullptr));
    startDeck.push_back(new Card("Plant Whip", AttackType::Plant, 6, 3, nullptr));
    startDeck.push_back(new Card("Neutral Punch", AttackType::Neutral, 3, 0, nullptr));

    // criar instância única do Player
    mPlayer = new Player(this, "Frog Hero", 20, 20, startDeck);

    // configurar owners das cartas
    for (Card* card : mPlayer->GetDeck()) {
        card->SetOwner(mPlayer);
    }

    SDL_Log("Player instance created via Game::InitializeActors");
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
    // Update fade (handles scene transition)
    UpdateFade(deltaTime);

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
    mPendingScene = scene;

    // Iniciar fade OUT automaticamente
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
        mFadeAlpha = t;

        if (t >= 1.0f)
        {
            mFadeOut = false;
            mFadeTimer = 0.0f;

            // Trocar cena agora que está escuro
            if (mPendingScene)
            {
                if (mCurrentScene)
                {
                    mCurrentScene->Exit();
                    delete mCurrentScene;
                }

                mIsChangingScene = true;
                std::vector<Actor*> actorsToDelete = mActors;
                mActors.clear();
                mPendingActors.clear();

                for (auto actor : actorsToDelete)
                {
                    // NÃO DELETAR O PLAYER AQUI SE NA LISTA DE ATORES
                    // pois ele só vai ser deletado se o jogador morrer ou no game::Shutdown
                    if (actor != mPlayer) {
                        delete actor;
                    }
                }

                // IMPORTANTE: Manter o Player na lista de atores vivos para o Update funcionar
                if (mPlayer) {
                    mActors.push_back(mPlayer);
                }

                mIsChangingScene = false;

                mCurrentScene = mPendingScene;
                mCurrentScene->Enter();
                mPendingScene = nullptr;

                // Iniciar fade IN apenas se trocamos de cena
                StartFade(false, mFadeDuration);
            }
            else
            {
                // Sem cena pendente, apenas terminar o fade (para casos como sair do jogo)
                mIsFading = false;
                mFadeAlpha = 1.0f;  // Manter tela preta
            }
        }
    }
    else
    {
        mFadeAlpha = 1.0f - t;

        if (t >= 1.0f)
        {
            mIsFading = false;
            mFadeAlpha = 0.0f;
        }
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

    // Deletar atores marcados como Destroy
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
    // Prevenir remoção durante transição de cena
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

    // Render fade overlay
    RenderFade();

    // Swap front buffer and back buffer
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

    // Limpar atores ANTES das cenas para evitar acessos inválidos
    while (!mActors.empty()) {
        Actor* actor = mActors.back();
        mActors.pop_back();
        delete actor;
    }
    mPendingActors.clear();

    // limpeza do player
    // O mPlayer provavelmente já foi deletado no loop acima se ele estava em mActors
    // Mas se por algum motivo foi removido da lista mas não deletado:
    if (mPlayer) {
        // Assumindo que o loop acima limpou tudo, apenas setamos null.
        // Se mPlayer NÃO estiver em mActors (ex: removido manualmente), delete aqui.
        // Para segurança, vamos assumir que o loop acima cuidou dele.

        // O destrutor do Player deleta as cartas, entao n precisamos deletar aq

        // Como o loop acima deleta TODOS (assim esperamos ;-;) os atores, e Player é um ator, ele já foi deletado.
        mPlayer = nullptr;
    }

    // Limpar cenas (SEM chamar Exit() pois os atores já foram deletados)
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