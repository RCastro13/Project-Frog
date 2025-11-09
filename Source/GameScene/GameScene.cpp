//
// Created by Mi PC on 05/11/2025.
//

#include "GameScene.h"
#include "../Game.h"
#include <SDL.h>
#include <SDL_log.h>

// ============================================
// CLASSE BASE: GameScene
// ============================================

GameScene::GameScene(Game* game)
    : mGame(game)
    , mStateTime(0.0f)
{
}

GameScene::~GameScene()
{
}

// ============================================
// MAIN MENU SCENE
// ============================================

MainMenuScene::MainMenuScene(Game* game)
    : GameScene(game)
{
}

MainMenuScene::~MainMenuScene()
{
}

void MainMenuScene::Enter()
{
    SDL_Log("Entering MainMenuScene");
    mStateTime = 0.0f;

    // Atualizar título da janela para indicar cena ativa
    SDL_SetWindowTitle(mGame->GetWindow(), "Project Frog - MAIN MENU [1=Mapa 2=Combate ESC=Sair]");

    // Cor de fundo: Azul escuro (como o Mario original)
    mGame->GetRenderer()->SetClearColor(0.0f, 0.3f, 0.6f, 1.0f);

    // TODO (Kayque): Carregar assets do menu
    // TODO (Kayque): Criar botões (Novo Jogo, Sair)
    // TODO (Kayque): Carregar música do menu
}

void MainMenuScene::Update(float deltaTime)
{
    mStateTime += deltaTime;

    // TODO (Kayque): Atualizar animações do menu
    // TODO (Kayque): Verificar seleção de botões
}

void MainMenuScene::ProcessInput(const Uint8* keyState)
{
    // TODO (Kayque): Implementar navegação do menu

    // ===== TESTE TEMPORÁRIO =====
    // Pressione 1 para ir ao Mapa
    // Pressione 2 para ir ao Combate
    // Pressione ESC para sair

    static bool key1WasPressed = false;
    static bool key2WasPressed = false;

    if (keyState[SDL_SCANCODE_1] && !key1WasPressed)
    {
        SDL_Log("==> TESTE: Transição Menu -> Mapa");
        mGame->SetScene(new MapScene(mGame));
        key1WasPressed = true;
    }
    else if (!keyState[SDL_SCANCODE_1])
    {
        key1WasPressed = false;
    }

    if (keyState[SDL_SCANCODE_2] && !key2WasPressed)
    {
        SDL_Log("==> TESTE: Transição Menu -> Combate");
        mGame->SetScene(new CombatScene(mGame));
        key2WasPressed = true;
    }
    else if (!keyState[SDL_SCANCODE_2])
    {
        key2WasPressed = false;
    }

    if (keyState[SDL_SCANCODE_ESCAPE])
    {
        SDL_Log("==> TESTE: Saindo do jogo");
        mGame->Quit();
    }
}

void MainMenuScene::Exit()
{
    SDL_Log("Exiting MainMenuScene");

    // TODO (Kayque): Limpar recursos do menu
    // TODO (Kayque): Parar música do menu
}

// ============================================
// MAP SCENE
// ============================================

MapScene::MapScene(Game* game)
    : GameScene(game)
{
}

MapScene::~MapScene()
{
}

void MapScene::Enter()
{
    SDL_Log("Entering MapScene");
    mStateTime = 0.0f;

    // Atualizar título da janela
    SDL_SetWindowTitle(mGame->GetWindow(), "Project Frog - MAPA [ENTER=Combate ESC=Menu]");

    // Cor de fundo: Verde (tema natureza/pântano)
    mGame->GetRenderer()->SetClearColor(0.2f, 0.5f, 0.3f, 1.0f);

    // TODO (Rubens): Carregar mapa
    // TODO (Rubens): Gerar/carregar nós do mapa
    // TODO (Rubens): Marcar nós disponíveis
    // TODO (Rubens): Carregar música do mapa
}

void MapScene::Update(float deltaTime)
{
    mStateTime += deltaTime;

    // TODO (Rubens): Atualizar animações do mapa
}

void MapScene::ProcessInput(const Uint8* keyState)
{
    // TODO (Rubens): Implementar navegação entre nós
    // TODO (Rubens): Implementar seleção de nó com Enter

    // ===== TESTE TEMPORÁRIO =====
    // Pressione ENTER para simular seleção de combate
    // Pressione ESC para voltar ao menu

    static bool enterWasPressed = false;
    static bool escWasPressed = false;

    if (keyState[SDL_SCANCODE_RETURN] && !enterWasPressed)
    {
        SDL_Log("==> TESTE: Transição Mapa -> Combate (nó selecionado)");
        mGame->SetScene(new CombatScene(mGame));
        enterWasPressed = true;
    }
    else if (!keyState[SDL_SCANCODE_RETURN])
    {
        enterWasPressed = false;
    }

    if (keyState[SDL_SCANCODE_ESCAPE] && !escWasPressed)
    {
        SDL_Log("==> TESTE: Transição Mapa -> Menu");
        mGame->SetScene(new MainMenuScene(mGame));
        escWasPressed = true;
    }
    else if (!keyState[SDL_SCANCODE_ESCAPE])
    {
        escWasPressed = false;
    }
}

void MapScene::Exit()
{
    SDL_Log("Exiting MapScene");

    // TODO (Rubens): Limpar recursos do mapa
    // TODO (Rubens): Parar música do mapa
}

// ============================================
// COMBAT SCENE
// ============================================

CombatScene::CombatScene(Game* game)
    : GameScene(game)
{
}

CombatScene::~CombatScene()
{
}

void CombatScene::Enter()
{
    SDL_Log("Entering CombatScene");
    mStateTime = 0.0f;

    // Atualizar título da janela
    SDL_SetWindowTitle(mGame->GetWindow(), "Project Frog - COMBATE [V=Vitoria D=Derrota ESC=Mapa]");

    // Cor de fundo: Vermelho escuro (tema batalha)
    mGame->GetRenderer()->SetClearColor(0.5f, 0.2f, 0.2f, 1.0f);

    // TODO (Facundo): Carregar background de combate
    // TODO (Facundo): Criar player e inimigo
    // TODO (Facundo): Criar cartas do player
    // TODO (Facundo): Iniciar sistema de turnos
    // TODO (Facundo): Carregar música de combate
}

void CombatScene::Update(float deltaTime)
{
    mStateTime += deltaTime;

    // TODO (Facundo): Atualizar sistema de combate
    // TODO (Facundo): Atualizar animações
    // TODO (Facundo): Verificar condições de vitória/derrota
}

void CombatScene::ProcessInput(const Uint8* keyState)
{
    // TODO (Facundo): Implementar seleção de cartas (WASD/Setas)
    // TODO (Facundo): Implementar confirmação de carta (Enter)
    // TODO (Facundo): Implementar opção de defesa

    // ===== TESTE TEMPORÁRIO =====
    // Pressione V para simular vitória
    // Pressione D para simular derrota
    // Pressione ESC para voltar ao mapa

    static bool vWasPressed = false;
    static bool dWasPressed = false;
    static bool escWasPressed = false;

    if (keyState[SDL_SCANCODE_V] && !vWasPressed)
    {
        SDL_Log("==> TESTE: Combate -> Vitória");
        mGame->SetScene(new VictoryScene(mGame));
        vWasPressed = true;
    }
    else if (!keyState[SDL_SCANCODE_V])
    {
        vWasPressed = false;
    }

    if (keyState[SDL_SCANCODE_D] && !dWasPressed)
    {
        SDL_Log("==> TESTE: Combate -> Derrota (Game Over)");
        mGame->SetScene(new GameOverScene(mGame));
        dWasPressed = true;
    }
    else if (!keyState[SDL_SCANCODE_D])
    {
        dWasPressed = false;
    }

    if (keyState[SDL_SCANCODE_ESCAPE] && !escWasPressed)
    {
        SDL_Log("==> TESTE: Combate -> Mapa");
        mGame->SetScene(new MapScene(mGame));
        escWasPressed = true;
    }
    else if (!keyState[SDL_SCANCODE_ESCAPE])
    {
        escWasPressed = false;
    }
}

void CombatScene::Exit()
{
    SDL_Log("Exiting CombatScene");

    // TODO (Facundo): Limpar recursos do combate
    // TODO (Facundo): Deletar inimigo
    // TODO (Facundo): Parar música de combate
}

// ============================================
// GAME OVER SCENE
// ============================================

GameOverScene::GameOverScene(Game* game)
    : GameScene(game)
{
}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::Enter()
{
    SDL_Log("Entering GameOverScene");
    mStateTime = 0.0f;

    // Atualizar título da janela
    SDL_SetWindowTitle(mGame->GetWindow(), "Project Frog - GAME OVER [ENTER=Menu]");

    // Cor de fundo: Roxo escuro (derrota)
    mGame->GetRenderer()->SetClearColor(0.3f, 0.2f, 0.4f, 1.0f);

    // TODO: Carregar background de game over
    // TODO: Exibir mensagem de derrota
    // TODO: Criar botão "Voltar ao Menu"
    // TODO: Carregar música de game over
}

void GameOverScene::Update(float deltaTime)
{
    mStateTime += deltaTime;

    // TODO: Atualizar animações (fade in, etc)
}

void GameOverScene::ProcessInput(const Uint8* keyState)
{
    // TODO: Implementar volta ao menu

    // ===== TESTE TEMPORÁRIO =====
    // Pressione ENTER para voltar ao menu

    static bool enterWasPressed = false;

    if (keyState[SDL_SCANCODE_RETURN] && !enterWasPressed)
    {
        SDL_Log("==> TESTE: Game Over -> Menu");
        mGame->SetScene(new MainMenuScene(mGame));
        enterWasPressed = true;
    }
    else if (!keyState[SDL_SCANCODE_RETURN])
    {
        enterWasPressed = false;
    }
}

void GameOverScene::Exit()
{
    SDL_Log("Exiting GameOverScene");

    // TODO: Limpar recursos
    // TODO: Parar música
}

// ============================================
// VICTORY SCENE
// ============================================

VictoryScene::VictoryScene(Game* game)
    : GameScene(game)
{
}

VictoryScene::~VictoryScene()
{
}

void VictoryScene::Enter()
{
    SDL_Log("Entering VictoryScene");
    mStateTime = 0.0f;

    // Atualizar título da janela
    SDL_SetWindowTitle(mGame->GetWindow(), "Project Frog - VITORIA! [ENTER=Menu]");

    // Cor de fundo: Dourado/Amarelo (vitória)
    mGame->GetRenderer()->SetClearColor(0.8f, 0.7f, 0.2f, 1.0f);

    // TODO: Carregar background de vitória
    // TODO: Exibir mensagem de vitória
    // TODO: Mostrar estatísticas da run
    // TODO: Criar botão "Voltar ao Menu"
    // TODO: Carregar música de vitória
}

void VictoryScene::Update(float deltaTime)
{
    mStateTime += deltaTime;

    // TODO: Atualizar animações
}

void VictoryScene::ProcessInput(const Uint8* keyState)
{
    // TODO: Implementar volta ao menu

    // ===== TESTE TEMPORÁRIO =====
    // Pressione ENTER para voltar ao menu

    static bool enterWasPressed = false;

    if (keyState[SDL_SCANCODE_RETURN] && !enterWasPressed)
    {
        SDL_Log("==> TESTE: Vitória -> Menu");
        mGame->SetScene(new MainMenuScene(mGame));
        enterWasPressed = true;
    }
    else if (!keyState[SDL_SCANCODE_RETURN])
    {
        enterWasPressed = false;
    }
}

void VictoryScene::Exit()
{
    SDL_Log("Exiting VictoryScene");

    // TODO: Limpar recursos
    // TODO: Parar música
}