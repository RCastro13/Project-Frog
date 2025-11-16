#include "NPCActor.h"
#include "../Components/Drawing/AnimatorComponent.h"
#include "../Game.h"

NPCActor::NPCActor(Game* game,
                   const char* texturePath,
                   const char* jsonPath,
                   const std::vector<int>& idleFrames,
                   int width,
                   int height)
    : Actor(game)
    , mAnimatorIdle(nullptr)
    , mWidth(width)
    , mHeight(height)
{
    // Criar AnimatorComponent para idle
    mAnimatorIdle = new AnimatorComponent(this, texturePath, jsonPath, width, height, 200);
    mAnimatorIdle->AddAnimation("idle", idleFrames);
    mAnimatorIdle->SetAnimFPS(8.0f); // 8 FPS para animação suave de idle
    mAnimatorIdle->SetAnimation("idle");
    mAnimatorIdle->SetEnabled(true);
}

NPCActor::~NPCActor()
{
}

void NPCActor::OnUpdate(float deltaTime)
{
    Actor::OnUpdate(deltaTime);
    // NPCs apenas mantêm sua animação idle rodando
}

void NPCActor::PlayIdle()
{
    if (mAnimatorIdle)
    {
        mAnimatorIdle->SetEnabled(true);
        mAnimatorIdle->SetAnimation("idle");
    }
}

