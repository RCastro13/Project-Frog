#pragma once
#include "Actor.h"
#include <vector>

class NPCActor : public Actor
{
public:
    NPCActor(Game* game,
             const char* texturePath,
             const char* jsonPath,
             const std::vector<int>& idleFrames,
             int width,
             int height);
    virtual ~NPCActor();

    void OnUpdate(float deltaTime) override;

    void PlayIdle();

protected:
    class AnimatorComponent* mAnimatorIdle;
    int mWidth;
    int mHeight;
};

