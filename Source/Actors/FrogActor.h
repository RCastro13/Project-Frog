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

    // Verifica se está em animação (não idle)
    bool IsInAnimation() const;

private:
    class AnimatorComponent* mAnimatorIdle;
    class AnimatorComponent* mAnimatorAttack;

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

