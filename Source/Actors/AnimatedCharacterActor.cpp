#include "AnimatedCharacterActor.h"
#include "ActorConstants.h"
#include "../Game.h"
#include "../Components/Drawing/AnimatorComponent.h"

AnimatedCharacterActor::AnimatedCharacterActor(Game* game,
                                             const char* idleTexturePath,
                                             const char* idleJsonPath,
                                             const char* attackTexturePath,
                                             const char* attackJsonPath,
                                             const std::vector<int>& idleFrames,
                                             const std::vector<int>& attackFrames,
                                             const std::vector<int>& hurtFrames,
                                             const std::vector<int>& deathFrames)
    : Actor(game)
    , mAnimatorIdle(nullptr)
    , mAnimatorAttack(nullptr)
    , mCurrentState(AnimState::Idle)
    , mAttackTimer(0.0f)
{
    using namespace ActorConstants;

    mAnimatorIdle = new AnimatorComponent(this,
        idleTexturePath,
        idleJsonPath,
        Sprites::CHARACTER_WIDTH,
        Sprites::CHARACTER_HEIGHT,
        Sprites::DEFAULT_DRAW_ORDER);

    mAnimatorIdle->AddAnimation("idle", idleFrames);
    mAnimatorIdle->AddAnimation("hurt", hurtFrames);
    mAnimatorIdle->AddAnimation("death", deathFrames);
    mAnimatorIdle->SetAnimFPS(Animation::DEFAULT_FPS);

    mAnimatorAttack = new AnimatorComponent(this,
        attackTexturePath,
        attackJsonPath,
        Sprites::CHARACTER_WIDTH,
        Sprites::CHARACTER_HEIGHT,
        Sprites::DEFAULT_DRAW_ORDER);

    mAnimatorAttack->AddAnimation("attack", attackFrames);
    mAnimatorAttack->SetAnimFPS(Animation::DEFAULT_FPS);
    mAnimatorAttack->SetEnabled(false);

    PlayIdle();
}

AnimatedCharacterActor::~AnimatedCharacterActor()
{
}

void AnimatedCharacterActor::OnUpdate(float deltaTime)
{
    if (mCurrentState == AnimState::Attack ||
        mCurrentState == AnimState::Hurt ||
        mCurrentState == AnimState::Death)
    {
        mAttackTimer -= deltaTime;
        if (mAttackTimer <= 0.0f)
        {
            PlayIdle();
        }
    }
}

void AnimatedCharacterActor::PlayIdle()
{
    if (mAnimatorIdle && mAnimatorAttack)
    {
        mCurrentState = AnimState::Idle;
        mAnimatorIdle->SetEnabled(true);
        mAnimatorAttack->SetEnabled(false);
        mAnimatorIdle->SetAnimation("idle");
    }
}

void AnimatedCharacterActor::PlayAttack()
{
    if (mAnimatorIdle && mAnimatorAttack)
    {
        mCurrentState = AnimState::Attack;
        mAnimatorIdle->SetEnabled(false);
        mAnimatorAttack->SetEnabled(true);
        mAnimatorAttack->SetAnimation("attack");
        mAttackTimer = ActorConstants::Animation::ANIMATION_DURATION;
    }
}

void AnimatedCharacterActor::PlayHurt()
{
    if (mAnimatorIdle && mAnimatorAttack)
    {
        mCurrentState = AnimState::Hurt;
        mAnimatorIdle->SetEnabled(true);
        mAnimatorAttack->SetEnabled(false);
        mAnimatorIdle->SetAnimation("hurt");
        mAttackTimer = ActorConstants::Animation::ANIMATION_DURATION;
    }
}

void AnimatedCharacterActor::PlayDeath()
{
    if (mAnimatorIdle && mAnimatorAttack)
    {
        mCurrentState = AnimState::Death;
        mAnimatorIdle->SetEnabled(true);
        mAnimatorAttack->SetEnabled(false);
        mAnimatorIdle->SetAnimation("death");
        mAttackTimer = ActorConstants::Animation::ANIMATION_DURATION;
    }
}

bool AnimatedCharacterActor::IsInAnimation() const
{
    return mCurrentState != AnimState::Idle;
}

