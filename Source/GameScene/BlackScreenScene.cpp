#include "BlackScreenScene.h"
#include "../Game.h"
#include "../Renderer/Renderer.h"

BlackScreenScene::BlackScreenScene(Game* game)
    : GameScene(game)
{
}

BlackScreenScene::~BlackScreenScene()
{
}

void BlackScreenScene::Enter()
{
    mStateTime = 0.0f;
}

void BlackScreenScene::Update(float deltaTime)
{
    mStateTime += deltaTime;

    if (mStateTime >= 0.5f)
    {
        mGame->Quit();
    }
}

void BlackScreenScene::RenderBackground()
{
    mGame->GetRenderer()->SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void BlackScreenScene::ProcessInput(const Uint8* keyState)
{
}

void BlackScreenScene::Exit()
{
}

