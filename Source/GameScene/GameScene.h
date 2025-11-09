//
// Created by Mi PC on 05/11/2025.
//

#pragma once
#include <SDL_stdinc.h>

// Forward declaration
class Game;

// Enum para identificar tipos de cena
enum class SceneType
{
    MAIN_MENU,
    MAP,
    COMBAT,
    GAME_OVER,
    VICTORY
};

// Classe base abstrata para todas as cenas do jogo
class GameScene
{
public:
    GameScene(Game* game);
    virtual ~GameScene();

    // Chamado quando a cena é ativada
    virtual void Enter() = 0;

    // Chamado a cada frame enquanto a cena está ativa
    virtual void Update(float deltaTime) = 0;

    // Chamado quando o jogador pressiona teclas
    virtual void ProcessInput(const Uint8* keyState) = 0;

    // Chamado quando a cena é desativada
    virtual void Exit() = 0;

    // Retorna o tipo da cena
    virtual SceneType GetType() const = 0;

    // Retorna o nome da cena (para debug)
    virtual const char* GetName() const = 0;

protected:
    Game* mGame;
    float mStateTime;  // Tempo que a cena está ativa
};

// ============================================
// CENAS CONCRETAS (stubs para o Sprint 1)
// ============================================

class MainMenuScene : public GameScene
{
public:
    MainMenuScene(Game* game);
    ~MainMenuScene();

    void Enter() override;
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* keyState) override;
    void Exit() override;

    SceneType GetType() const override { return SceneType::MAIN_MENU; }
    const char* GetName() const override { return "MainMenu"; }
};

class MapScene : public GameScene
{
public:
    MapScene(Game* game);
    ~MapScene();

    void Enter() override;
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* keyState) override;
    void Exit() override;

    SceneType GetType() const override { return SceneType::MAP; }
    const char* GetName() const override { return "Map"; }
};

class CombatScene : public GameScene
{
public:
    CombatScene(Game* game);
    ~CombatScene();

    void Enter() override;
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* keyState) override;
    void Exit() override;

    SceneType GetType() const override { return SceneType::COMBAT; }
    const char* GetName() const override { return "Combat"; }
};

class GameOverScene : public GameScene
{
public:
    GameOverScene(Game* game);
    ~GameOverScene();

    void Enter() override;
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* keyState) override;
    void Exit() override;

    SceneType GetType() const override { return SceneType::GAME_OVER; }
    const char* GetName() const override { return "GameOver"; }
};

class VictoryScene : public GameScene
{
public:
    VictoryScene(Game* game);
    ~VictoryScene();

    void Enter() override;
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* keyState) override;
    void Exit() override;

    SceneType GetType() const override { return SceneType::VICTORY; }
    const char* GetName() const override { return "Victory"; }
};