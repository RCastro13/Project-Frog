#include "FrogActor.h"
#include "../Game.h"
#include "../Components/Drawing/AnimatorComponent.h"
#include <SDL_log.h>

FrogActor::FrogActor(Game* game)
    : Actor(game)
    , mAnimatorIdle(nullptr)
    , mAnimatorAttack(nullptr)
    , mCurrentState(AnimState::Idle)
    , mAttackTimer(0.0f)
{
    // Animator para idle, hurt e death
    mAnimatorIdle = new AnimatorComponent(this,
        "../Assets/Frog/Frog.png",
        "../Assets/Frog/Frog.json",
        96, 96, 100);

    mAnimatorIdle->AddAnimation("idle", {0, 1, 2, 3});
    mAnimatorIdle->AddAnimation("hurt", {5, 10, 13, 16});
    mAnimatorIdle->AddAnimation("death", {10, 11, 17, 14});
    mAnimatorIdle->SetAnimFPS(10.0f);

    // Animator para attack (mesmo arquivo)
    mAnimatorAttack = new AnimatorComponent(this,
        "../Assets/Frog/Frog.png",
        "../Assets/Frog/Frog.json",
        96, 96, 100);

    mAnimatorAttack->AddAnimation("attack", {17, 18, 20, 22});
    mAnimatorAttack->SetAnimFPS(10.0f);
    mAnimatorAttack->SetEnabled(false);

    PlayIdle();
}

FrogActor::~FrogActor()
{
}

void FrogActor::OnUpdate(float deltaTime)
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

void FrogActor::PlayIdle()
{
    if (mAnimatorIdle && mAnimatorAttack)
    {
        mCurrentState = AnimState::Idle;
        mAnimatorIdle->SetEnabled(true);
        mAnimatorAttack->SetEnabled(false);
        mAnimatorIdle->SetAnimation("idle");
    }
}

void FrogActor::PlayAttack()
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

void FrogActor::PlayHurt()
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

void FrogActor::PlayDeath()
{
    if (mAnimatorIdle && mAnimatorAttack)
    {
        mCurrentState = AnimState::Death;
        mAnimatorIdle->SetEnabled(true);
        mAnimatorAttack->SetEnabled(false);
        mAnimatorIdle->SetAnimation("death");
        mAttackTimer = 2.0f;
    }
}

bool FrogActor::IsInAnimation() const
{
    return mCurrentState != AnimState::Idle;
}

