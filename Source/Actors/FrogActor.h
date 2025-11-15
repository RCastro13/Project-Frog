#pragma once
#include "Actor.h"

class FrogActor : public Actor
{
public:
    FrogActor(Game* game);
    ~FrogActor();

    void OnUpdate(float deltaTime) override;

    // Controle de animações
    void PlayIdle();
    void PlayAttack();
    void PlayHurt();
    void PlayDeath();

private:
    class AnimatorComponent* mAnimator;

    // Estado da animação
    enum class AnimState
    {
        Idle,
        Attack,
        Hurt,
        Death
    };

    AnimState mCurrentState;
    float mAttackTimer;  // Timer para retornar ao idle após ataque
};

