#include "AnimationTestScene.h"
#include "MainMenuScene.h"
#include "../Game.h"
#include "../Actors/BearActor.h"
#include "../Actors/BeatleActor.h"
#include "../Actors/SnakeActor.h"
#include "../Actors/WolfActor.h"
#include "../Actors/AnimatedCharacterActor.h"
#include "../Renderer/Texture.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/Font.h"
#include <SDL.h>

AnimationTestScene::AnimationTestScene(Game* game)
    : GameScene(game)
    , mBackgroundTexture(nullptr)
    , mCurrentAnimation(AnimationType::IDLE)
    , mKeyWasPressed(false)
{
}

AnimationTestScene::~AnimationTestScene()
{
}

void AnimationTestScene::Enter()
{
    CreateTestActors();
    PlayAnimation(AnimationType::IDLE);
}

void AnimationTestScene::CreateTestActors()
{
    // Posições para os 4 inimigos (distribuídos horizontalmente)
    const float baseY = 224.0f;
    const float spacing = 150.0f;
    const float startX = 120.0f;

    // Bear
    BearActor* bear = new BearActor(mGame);
    bear->SetPosition(Vector2(startX, baseY));
    mTestActors.push_back(bear);

    // Beatle
    BeatleActor* beatle = new BeatleActor(mGame);
    beatle->SetPosition(Vector2(startX + spacing, baseY));
    mTestActors.push_back(beatle);

    // Snake
    SnakeActor* snake = new SnakeActor(mGame);
    snake->SetPosition(Vector2(startX + spacing * 2, baseY));
    mTestActors.push_back(snake);

    // Wolf
    WolfActor* wolf = new WolfActor(mGame);
    wolf->SetPosition(Vector2(startX + spacing * 3, baseY));
    mTestActors.push_back(wolf);
}

void AnimationTestScene::PlayAnimation(AnimationType type)
{
    mCurrentAnimation = type;

    for (AnimatedCharacterActor* actor : mTestActors)
    {
        switch (type)
        {
            case AnimationType::IDLE:
                actor->PlayIdle();
                break;
            case AnimationType::ATTACK:
                actor->PlayAttack();
                break;
            case AnimationType::HURT:
                actor->PlayHurt();
                break;
            case AnimationType::DEATH:
                actor->PlayDeath();
                break;
            default:
                break;
        }
    }
}

void AnimationTestScene::Update(float deltaTime)
{
    mStateTime += deltaTime;
    UpdateFade(deltaTime);
}

void AnimationTestScene::RenderBackground()
{
    if (mBackgroundTexture)
    {
        mGame->GetRenderer()->DrawTexture(
            Vector2(320.0f, 224.0f),
            Vector2(640.0f, 448.0f),
            0.0f,
            Vector3(1.0f, 1.0f, 1.0f),
            mBackgroundTexture,
            Vector4::UnitRect,
            Vector2::Zero
        );
    }
}

void AnimationTestScene::Render()
{
    RenderBackground();
    RenderInstructions();
}

void AnimationTestScene::RenderInstructions()
{
    if (!mGame->GetFont()) return;

    Renderer* renderer = mGame->GetRenderer();

    // Título
    Texture* titleTexture = mGame->GetFont()->RenderText(
        "ANIMATION TEST - Enemy Sprites",
        Vector3(1.0f, 1.0f, 1.0f),
        32
    );
    if (titleTexture)
    {
        renderer->DrawTexture(
            Vector2(320.0f, 40.0f),
            Vector2(titleTexture->GetWidth(), titleTexture->GetHeight()),
            0.0f,
            Vector3(1.0f, 1.0f, 1.0f),
            titleTexture,
            Vector4::UnitRect,
            Vector2::Zero
        );
        delete titleTexture;
    }

    // Nomes dos inimigos
    const char* names[] = {"BEAR", "BEATLE", "SNAKE", "WOLF"};
    const float startX = 120.0f;
    const float spacing = 150.0f;
    const float nameY = 340.0f;

    for (int i = 0; i < 4; i++)
    {
        Texture* nameTexture = mGame->GetFont()->RenderText(
            names[i],
            Vector3(1.0f, 0.84f, 0.0f),
            20
        );
        if (nameTexture)
        {
            renderer->DrawTexture(
                Vector2(startX + spacing * i, nameY),
                Vector2(nameTexture->GetWidth(), nameTexture->GetHeight()),
                0.0f,
                Vector3(1.0f, 1.0f, 1.0f),
                nameTexture,
                Vector4::UnitRect,
                Vector2::Zero
            );
            delete nameTexture;
        }
    }

    // Controles
    const char* controls[] = {
        "1 - IDLE Animation",
        "2 - ATTACK Animation",
        "3 - HURT Animation",
        "4 - DEATH Animation",
        "ESC - Return to Menu"
    };

    float controlY = 380.0f;
    for (int i = 0; i < 5; i++)
    {
        Texture* controlTexture = mGame->GetFont()->RenderText(
            controls[i],
            Vector3(0.8f, 0.8f, 0.8f),
            18
        );
        if (controlTexture)
        {
            renderer->DrawTexture(
                Vector2(320.0f, controlY + i * 20.0f),
                Vector2(controlTexture->GetWidth(), controlTexture->GetHeight()),
                0.0f,
                Vector3(1.0f, 1.0f, 1.0f),
                controlTexture,
                Vector4::UnitRect,
                Vector2::Zero
            );
            delete controlTexture;
        }
    }

    // Animação atual
    const char* animNames[] = {"IDLE", "ATTACK", "HURT", "DEATH"};
    char currentAnimText[64];
    snprintf(currentAnimText, sizeof(currentAnimText), "Current: %s",
             animNames[static_cast<int>(mCurrentAnimation)]);

    Texture* currentTexture = mGame->GetFont()->RenderText(
        currentAnimText,
        Vector3(0.15f, 0.7f, 0.15f),
        24
    );
    if (currentTexture)
    {
        renderer->DrawTexture(
            Vector2(320.0f, 90.0f),
            Vector2(currentTexture->GetWidth(), currentTexture->GetHeight()),
            0.0f,
            Vector3(1.0f, 1.0f, 1.0f),
            currentTexture,
            Vector4::UnitRect,
            Vector2::Zero
        );
        delete currentTexture;
    }
}

void AnimationTestScene::ProcessInput(const Uint8* keyState)
{
    if (ShouldBlockInput())
        return;

    // ESC - voltar ao menu
    if (keyState[SDL_SCANCODE_ESCAPE] && !mKeyWasPressed)
    {
        mKeyWasPressed = true;
        mGame->SetScene(new class MainMenuScene(mGame));
        return;
    }

    // Teclas 1-4 para trocar animações
    if (keyState[SDL_SCANCODE_1] && !mKeyWasPressed)
    {
        mKeyWasPressed = true;
        PlayAnimation(AnimationType::IDLE);
    }
    else if (keyState[SDL_SCANCODE_2] && !mKeyWasPressed)
    {
        mKeyWasPressed = true;
        PlayAnimation(AnimationType::ATTACK);
    }
    else if (keyState[SDL_SCANCODE_3] && !mKeyWasPressed)
    {
        mKeyWasPressed = true;
        PlayAnimation(AnimationType::HURT);
    }
    else if (keyState[SDL_SCANCODE_4] && !mKeyWasPressed)
    {
        mKeyWasPressed = true;
        PlayAnimation(AnimationType::DEATH);
    }

    // Reset do flag quando soltar a tecla
    if (!keyState[SDL_SCANCODE_ESCAPE] &&
        !keyState[SDL_SCANCODE_1] &&
        !keyState[SDL_SCANCODE_2] &&
        !keyState[SDL_SCANCODE_3] &&
        !keyState[SDL_SCANCODE_4])
    {
        mKeyWasPressed = false;
    }
}

void AnimationTestScene::Exit()
{
    // Limpar atores
    for (AnimatedCharacterActor* actor : mTestActors)
    {
        if (actor)
        {
            actor->SetState(ActorState::Destroy);
        }
    }
    mTestActors.clear();
}
