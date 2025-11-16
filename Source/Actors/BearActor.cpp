#include "BearActor.h"
#include "../Game.h"
#include "../Components/Drawing/AnimatorComponent.h"
#include <SDL_log.h>

BearActor::BearActor(Game* game)
    : Actor(game)
    , mAnimatorIdle(nullptr)
    , mAnimatorAttack(nullptr)
    , mCurrentState(AnimState::Idle)
    , mAttackTimer(0.0f)
{
    mAnimatorIdle = new AnimatorComponent(this,
        "../Assets/Enemies/Bear/Bear.png",
        "../Assets/Enemies/Bear/Bear.json",
        96, 96, 100);

    mAnimatorIdle->AddAnimation("idle", {0, 1, 2, 3});
    mAnimatorIdle->AddAnimation("hurt", {48, 49, 50, 50});
    mAnimatorIdle->AddAnimation("death", {50, 51, 50, 51});
    mAnimatorIdle->SetAnimFPS(10.0f);

    mAnimatorAttack = new AnimatorComponent(this,
        "../Assets/Enemies/BearAttack/BearAttack.png",
        "../Assets/Enemies/BearAttack/BearAttack.json",
        96, 96, 100);

    mAnimatorAttack->AddAnimation("attack", {0, 1, 2, 3});
    mAnimatorAttack->SetAnimFPS(10.0f);
    mAnimatorAttack->SetEnabled(false);

    SetScale(Vector2(-1.0f, 1.0f));
    PlayIdle();
}

BearActor::~BearActor()
{
}

void BearActor::OnUpdate(float deltaTime)
{
    if (mCurrentState == AnimState::Attack || mCurrentState == AnimState::Hurt || mCurrentState == AnimState::Death)
    {
        mAttackTimer -= deltaTime;
        if (mAttackTimer <= 0.0f)
        {
            PlayIdle();
        }
    }
}

void BearActor::PlayIdle()
{
    if (mAnimatorIdle && mAnimatorAttack)
    {
        mCurrentState = AnimState::Idle;
        mAnimatorIdle->SetEnabled(true);
        mAnimatorAttack->SetEnabled(false);
        mAnimatorIdle->SetAnimation("idle");
    }
}

void BearActor::PlayAttack()
{
    if (mAnimatorIdle && mAnimatorAttack)
    {
        mCurrentState = AnimState::Attack;
        mAnimatorIdle->SetEnabled(false);
        mAnimatorAttack->SetEnabled(true);
        mAnimatorAttack->SetAnimation("attack");
        mAttackTimer = 2.0f;
    }
}

void BearActor::PlayHurt()
{
    if (mAnimatorIdle && mAnimatorAttack)
    {
        mCurrentState = AnimState::Hurt;
        mAnimatorIdle->SetEnabled(true);
        mAnimatorAttack->SetEnabled(false);
        mAnimatorIdle->SetAnimation("hurt");
        mAttackTimer = 2.0f;
    }
}

void BearActor::PlayDeath()
{
    if (mAnimatorIdle && mAnimatorAttack)
    {
        mCurrentState = AnimState::Death;
        mAnimatorIdle->SetEnabled(true);
        mAnimatorAttack->SetEnabled(false);
        mAnimatorIdle->SetAnimation("death");
        mAttackTimer = 2.0f; // Duração da animação
    }
}

bool BearActor::IsInAnimation() const
{
    return mCurrentState != AnimState::Idle;
}

