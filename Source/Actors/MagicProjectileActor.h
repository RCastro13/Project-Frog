#pragma once
#include "Actor.h"
#include "../Combat/Card.h"

class MagicProjectileActor : public Actor
{
public:
    MagicProjectileActor(Game* game, AttackType type, Vector2 startPos, Vector2 endPos, float travelTime, bool rotate180 = false);
    ~MagicProjectileActor();

    void OnUpdate(float deltaTime) override;

    bool IsComplete() const { return mIsComplete; }

private:
    class AnimatorComponent* mAnimator;

    Vector2 mStartPos;
    Vector2 mEndPos;
    float mTravelTime;
    float mTimer;
    bool mIsComplete;
    bool mRotate180;

    void LoadMagicAnimation(AttackType type);
};

