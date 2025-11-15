#include "FrogActor.h"
#include "../Game.h"
#include "../Components/Drawing/AnimatorComponent.h"

FrogActor::FrogActor(Game* game)
    : Actor(game)
    , mAnimator(nullptr)
    , mCurrentState(AnimState::Idle)
    , mAttackTimer(0.0f)
{
    // Criar AnimatorComponent com as sprites do sapo
    // Sprites são 24x24, vamos escalar para 96x96 (4x)
    mAnimator = new AnimatorComponent(this,
        "Assets/Frog/Frog.png",
        "Assets/Frog/Frog.json",
        96, 96, 100);

    // Definir animações (usando frames disponíveis no JSON)
    // Idle: frames 0-3 (parado/esperando)
    mAnimator->AddAnimation("idle", {0, 1, 2, 3});

    // Attack: frames 4-7 (ataque)
    mAnimator->AddAnimation("attack", {4, 5, 6, 7});

    // Hurt: frames 8-9 (tomando dano)
    mAnimator->AddAnimation("hurt", {8, 9});

    // Death: frames 10-11 (morrendo)
    mAnimator->AddAnimation("death", {10, 11});

    // Configurar velocidade da animação
    mAnimator->SetAnimFPS(8.0f);

    // Iniciar com animação idle
    PlayIdle();
}

FrogActor::~FrogActor()
{
    // AnimatorComponent será deletado automaticamente pelo Actor
}

void FrogActor::OnUpdate(float deltaTime)
{
    // Se estiver em ataque, contar o timer
    if (mCurrentState == AnimState::Attack)
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
    if (mAnimator)
    {
        mCurrentState = AnimState::Idle;
        mAnimator->SetAnimation("idle");
    }
}

void FrogActor::PlayAttack()
{
    if (mAnimator)
    {
        mCurrentState = AnimState::Attack;
        mAnimator->SetAnimation("attack");
        mAttackTimer = 0.8f; // Duração da animação de ataque
    }
}

void FrogActor::PlayHurt()
{
    if (mAnimator)
    {
        mCurrentState = AnimState::Hurt;
        mAnimator->SetAnimation("hurt");
        mAttackTimer = 0.4f; // Duração curta para hurt
    }
}

void FrogActor::PlayDeath()
{
    if (mAnimator)
    {
        mCurrentState = AnimState::Death;
        mAnimator->SetAnimation("death");
        mAnimator->SetIsPaused(true); // Pausar no último frame
    }
}

