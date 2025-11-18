#pragma once
#include "Actor.h"
#include <vector>

class Card;

class AnimatedCharacterActor : public Actor
{
public:
    // Estrutura para stats de inimigos
    struct EnemyStats {
        int health;
        int maxHealth;
        double difficulty;
    };

    // Métodos virtuais para configuração de cada inimigo
    virtual EnemyStats GetEnemyStats() const = 0;
    virtual std::vector<Card*> CreateEnemyDeck() const = 0;
    AnimatedCharacterActor(Game* game,
                          const char* idleTexturePath,
                          const char* idleJsonPath,
                          const char* attackTexturePath,
                          const char* attackJsonPath,
                          const std::vector<int>& idleFrames,
                          const std::vector<int>& attackFrames,
                          const std::vector<int>& hurtFrames,
                          const std::vector<int>& deathFrames);
    virtual ~AnimatedCharacterActor();

    void OnUpdate(float deltaTime) override;

    void PlayIdle();
    void PlayAttack();
    void PlayHurt();
    void PlayDeath();

    bool IsInAnimation() const;

protected:
    enum class AnimState
    {
        Idle,
        Attack,
        Hurt,
        Death
    };

    class AnimatorComponent* mAnimatorIdle;
    class AnimatorComponent* mAnimatorAttack;
    AnimState mCurrentState;
    float mAttackTimer;
};

