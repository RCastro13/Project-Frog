#pragma once
#include "Actor.h"
#include <vector>

class AnimatedCharacterActor : public Actor
{
public:
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

