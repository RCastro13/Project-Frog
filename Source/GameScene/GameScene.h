#pragma once
#include <SDL_stdinc.h>

class Game;

enum class SceneType
{
    MAIN_MENU,
    MAP,
    COMBAT,
    GAME_OVER,
    VICTORY,
    BLACK_SCREEN,
    REWARD,
    SHOP,
    REST
};

class GameScene
{
public:
    GameScene(Game* game);
    virtual ~GameScene();

    virtual void Enter() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void RenderBackground() {}
    virtual void Render() {}
    virtual void ProcessInput(const Uint8* keyState) = 0;
    virtual void Exit() = 0;

    virtual SceneType GetType() const = 0;
    virtual const char* GetName() const = 0;

protected:
    Game* mGame;
    float mStateTime;
    float mFadeAlpha;
    float mFadeTimer;
    bool mFadingIn;

    void UpdateFade(float deltaTime);
    void RenderFade();
    bool ShouldBlockInput() const;
};

class MainMenuScene;
class MapScene;
class CombatScene;
class GameOverScene;
class VictoryScene;
class BlackScreenScene;
