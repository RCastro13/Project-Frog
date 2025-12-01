#include "GameScene.h"
#include "../Game.h"

GameScene::GameScene(Game* game)
    : mGame(game)
    , mStateTime(0.0f)
    , mFadeAlpha(1.0f)
    , mFadeTimer(0.0f)
    , mFadingIn(true)
{
}

GameScene::~GameScene()
{
}

void GameScene::UpdateFade(float deltaTime)
{
    const float FADE_DURATION = 0.5f;

    if (mFadingIn)
    {
        mFadeTimer += deltaTime;
        mFadeAlpha = 1.0f - (mFadeTimer / FADE_DURATION);

        if (mFadeAlpha <= 0.0f)
        {
            mFadeAlpha = 0.0f;
            mFadingIn = false;
        }
    }
}

void GameScene::RenderFade()
{
}

bool GameScene::ShouldBlockInput() const
{
    return mFadeAlpha > 0.0f || mGame->IsFading();
}
