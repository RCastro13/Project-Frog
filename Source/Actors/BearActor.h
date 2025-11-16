#pragma once
#include "Actor.h"

class BearActor : public Actor
{
public:
    BearActor(Game* game);
    ~BearActor();

    void OnUpdate(float deltaTime) override;

    // Controle de animações
    void PlayIdle();
    void PlayAttack();
    void PlayHurt();
    void PlayDeath();

    // Verifica se está em animação (não idle)
    bool IsInAnimation() const;

private:
    class AnimatorComponent* mAnimatorIdle;    // Para idle, hurt, death (Bear.json)
    class AnimatorComponent* mAnimatorAttack;  // Para attack (BearAttack.json)

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

