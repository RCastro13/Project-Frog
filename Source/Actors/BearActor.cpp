#include "BearActor.h"
#include "../Game.h"
#include "../Components/Drawing/AnimatorComponent.h"

BearActor::BearActor(Game* game)
    : Actor(game)
    , mAnimator(nullptr)
    , mCurrentState(AnimState::Idle)
    , mAttackTimer(0.0f)
{
    // Criar AnimatorComponent com as sprites do urso
    // Sprites são 24x24, vamos escalar para 96x96 (4x)
    mAnimator = new AnimatorComponent(this,
        "Assets/Enemies/Bear/Bear.png",
        "Assets/Enemies/Bear/Bear.json",
        96, 96, 100);

    // Definir animações (usando frames disponíveis no JSON)
    // Idle: frames 0-7 (parado/esperando)
    mAnimator->AddAnimation("idle", {0, 1, 2, 3, 4, 5, 6, 7});

    // Attack: frames 8-15 (ataque)
    mAnimator->AddAnimation("attack", {8, 9, 10, 11, 12, 13, 14, 15});

    // Hurt: frames 16-19 (tomando dano)
    mAnimator->AddAnimation("hurt", {16, 17, 18, 19});

    // Death: frames 20-23 (morrendo)
    mAnimator->AddAnimation("death", {20, 21, 22, 23});

    // Configurar velocidade da animação
    mAnimator->SetAnimFPS(10.0f);

    // Iniciar com animação idle
    PlayIdle();
}

BearActor::~BearActor()
{
    // AnimatorComponent será deletado automaticamente pelo Actor
}

void BearActor::OnUpdate(float deltaTime)
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

void BearActor::PlayIdle()
{
    if (mAnimator)
    {
        mCurrentState = AnimState::Idle;
        mAnimator->SetAnimation("idle");
    }
}

void BearActor::PlayAttack()
{
    if (mAnimator)
    {
        mCurrentState = AnimState::Attack;
        mAnimator->SetAnimation("attack");
        mAttackTimer = 0.8f; // Duração da animação de ataque
    }
}

void BearActor::PlayHurt()
{
    if (mAnimator)
    {
        mCurrentState = AnimState::Hurt;
        mAnimator->SetAnimation("hurt");
        mAttackTimer = 0.4f; // Duração curta para hurt
    }
}

void BearActor::PlayDeath()
{
    if (mAnimator)
    {
        mCurrentState = AnimState::Death;
        mAnimator->SetAnimation("death");
        mAnimator->SetIsPaused(true); // Pausar no último frame
    }
}

