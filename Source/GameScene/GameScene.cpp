//
// Created by Mi PC on 05/11/2025.
//

#include "GameScene.h"
#include "../Game.h"
#include "../Combat/CombatManager.h"
#include "../Combat/Card.h"
#include "../Actors/Player.h"
#include "../Actors/Enemy.h"
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
    , mCombatManager(nullptr)
    , mPlayer(nullptr)
    , mEnemy(nullptr)
    , mSelectedCardIndex(0)
    , mKeyWasPressed(false)
    , mCardsShown(false)
{
}

CombatScene::~CombatScene()
{
    // Cleanup será feito no Exit()
}

void CombatScene::Enter()
{
    mStateTime = 0.0f;

    // Atualizar título da janela
    SDL_SetWindowTitle(mGame->GetWindow(), "Project Frog - COMBATE [Setas=Selecionar Enter=Confirmar]");

    // Cor de fundo: Vermelho escuro (tema batalha)
    mGame->GetRenderer()->SetClearColor(0.5f, 0.2f, 0.2f, 1.0f);

    // Criar combatentes e iniciar combate
    CreateTestCombatants();

    SDL_Log("\n========================================");
    SDL_Log("       COMBATE INICIADO");
    SDL_Log("========================================");
    SDL_Log("Player: %s (HP: %d)", "Frog Hero", mPlayer->GetHealth());
    SDL_Log("Enemy:  %s (HP: %d)", "Slime", mEnemy->GetHealth());
    SDL_Log("========================================\n");
}

void CombatScene::CreateTestCombatants()
{
    // Criar cartas de teste para o Player
    std::vector<Card*> playerDeck;
    playerDeck.push_back(new Card("Fire Strike", AttackType::Fire, 5, 2, nullptr));
    playerDeck.push_back(new Card("Water Shield", AttackType::Water, 4, 1, nullptr));
    playerDeck.push_back(new Card("Plant Whip", AttackType::Plant, 6, 3, nullptr));
    playerDeck.push_back(new Card("Neutral Punch", AttackType::Neutral, 3, 0, nullptr));

    // Criar cartas de teste para o Enemy
    std::vector<Card*> enemyDeck;
    enemyDeck.push_back(new Card("Enemy Fire", AttackType::Fire, 4, 1, nullptr));
    enemyDeck.push_back(new Card("Enemy Water", AttackType::Water, 5, 2, nullptr));
    enemyDeck.push_back(new Card("Enemy Plant", AttackType::Plant, 4, 2, nullptr));
    enemyDeck.push_back(new Card("Enemy Neutral", AttackType::Neutral, 3, 0, nullptr));

    // Criar Player e Enemy
    mPlayer = new Player(mGame, "Frog Hero", 20, 20, playerDeck);
    mEnemy = new Enemy(mGame, "Slime", 15, 15, enemyDeck);

    // Configurar owners das cartas
    for (Card* card : mPlayer->GetDeck()) {
        card->SetOwner(mPlayer);
    }
    for (Card* card : mEnemy->GetDeck()) {
        card->SetOwner(mEnemy);
    }

    // Criar CombatManager
    mCombatManager = new CombatManager(mPlayer, mEnemy, 10); // 10 moedas de recompensa
    mCombatManager->StartCombat();

    mSelectedCardIndex = 0;
}

void CombatScene::Update(float deltaTime)
{
    mStateTime += deltaTime;

    if (!mCombatManager)
        return;

    // Guardar estado anterior para detectar mudanças
    static CombatState previousState = CombatState::WAITING_FOR_PLAYER;
    CombatState currentState = mCombatManager->GetCurrentState();

    // Atualizar o gerenciador de combate
    mCombatManager->Update();

    // Logar após completar um turno (quando volta a esperar o player)
    if (previousState != CombatState::WAITING_FOR_PLAYER &&
        currentState == CombatState::WAITING_FOR_PLAYER)
    {
        SDL_Log("----------------------------------------");
        SDL_Log("Player HP: %d | Enemy HP: %d",
                mPlayer->GetHealth(), mEnemy->GetHealth());
        SDL_Log("----------------------------------------\n");

        // Reset flag para mostrar cartas novamente
        mCardsShown = false;
    }

    // Mostrar cartas disponíveis quando estiver esperando o player
    if (currentState == CombatState::WAITING_FOR_PLAYER && !mCardsShown)
    {
        LogAvailableCards();
        mCardsShown = true;
    }

    previousState = currentState;

    // Verificar fim do combate
    if (mCombatManager->IsCombatEnded())
    {
        HandleCombatEnd();
    }
}

void CombatScene::RenderCombatUI()
{
    // TODO (Rubens): Adicionar renderização visual com sprites/texto na tela
    // Por enquanto, mantemos apenas os logs essenciais
}

void CombatScene::ProcessInput(const Uint8* keyState)
{
    if (!mCombatManager)
        return;

    // Só processar input se estiver esperando o jogador
    if (mCombatManager->GetCurrentState() != CombatState::WAITING_FOR_PLAYER)
        return;

    // Navegação entre cartas (Left/Right ou A/D)
    if ((keyState[SDL_SCANCODE_RIGHT] || keyState[SDL_SCANCODE_D]) && !mKeyWasPressed)
    {
        mSelectedCardIndex = (mSelectedCardIndex + 1) % 4;
        LogAvailableCards(); // Atualizar display
        mKeyWasPressed = true;
    }
    else if ((keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A]) && !mKeyWasPressed)
    {
        mSelectedCardIndex = (mSelectedCardIndex - 1 + 4) % 4;
        LogAvailableCards(); // Atualizar display
        mKeyWasPressed = true;
    }
    // Confirmar seleção (Enter ou Space)
    else if ((keyState[SDL_SCANCODE_RETURN] || keyState[SDL_SCANCODE_SPACE]) && !mKeyWasPressed)
    {
        Card* selectedCard = mPlayer->GetDeck()[mSelectedCardIndex];

        // Verificar se a carta está disponível (não em cooldown)
        if (selectedCard->IsAvailable())
        {
            SDL_Log("TURNO %d", mCombatManager->GetCurrentTurn());
            SDL_Log("Player usou: %s (Tipo: %s, Dano: %d)",
                    selectedCard->GetName().c_str(),
                    GetTypeName(selectedCard->GetType()),
                    selectedCard->GetDamage());
            mCombatManager->PlayerSelectCard(selectedCard);
            mKeyWasPressed = true;
        }
        else
        {
            SDL_Log("❌ Carta em cooldown! (%d turnos restantes)",
                    selectedCard->GetCurrentCooldown());
            mKeyWasPressed = true;
        }
    }
    // Reset do flag quando nenhuma tecla está pressionada
    else if (!keyState[SDL_SCANCODE_RIGHT] && !keyState[SDL_SCANCODE_LEFT] &&
             !keyState[SDL_SCANCODE_D] && !keyState[SDL_SCANCODE_A] &&
             !keyState[SDL_SCANCODE_RETURN] && !keyState[SDL_SCANCODE_SPACE])
    {
        mKeyWasPressed = false;
    }

    // ESC para voltar ao menu (apenas para teste)
    static bool escWasPressed = false;
    if (keyState[SDL_SCANCODE_ESCAPE] && !escWasPressed)
    {
        SDL_Log("\n[Voltando ao menu principal]\n");
        mGame->SetScene(new MainMenuScene(mGame));
        escWasPressed = true;
    }
    else if (!keyState[SDL_SCANCODE_ESCAPE])
    {
        escWasPressed = false;
    }
}

void CombatScene::LogAvailableCards()
{
    SDL_Log("Suas cartas: (A=Anterior D=Próximo)");

    for (int i = 0; i < 4; i++)
    {
        Card* card = mPlayer->GetDeck()[i];
        const char* selector = (i == mSelectedCardIndex) ? "👉 " : "   ";

        if (card->IsAvailable())
        {
            SDL_Log("%s[%d] %s (Tipo: %s, Dano: %d)",
                    selector, i + 1,
                    card->GetName().c_str(),
                    GetTypeName(card->GetType()),
                    card->GetDamage());
        }
        else
        {
            SDL_Log("%s[%d] %s ⏳ COOLDOWN (%d turnos)",
                    selector, i + 1,
                    card->GetName().c_str(),
                    card->GetCurrentCooldown());
        }
    }
    SDL_Log("");
}

const char* CombatScene::GetTypeName(AttackType type)
{
    switch (type)
    {
        case AttackType::Fire:    return "🔥 Fire";
        case AttackType::Water:   return "💧 Water";
        case AttackType::Plant:   return "🌿 Plant";
        case AttackType::Neutral: return "⚪ Neutral";
        default:                  return "Unknown";
    }
}

void CombatScene::HandleCombatEnd()
{
    SDL_Log("\n========================================");
    if (mCombatManager->IsPlayerVictorious())
    {
        SDL_Log("       ✨ VITÓRIA! ✨");
        SDL_Log("========================================");
        SDL_Log("Recompensa: %d moedas", mCombatManager->GetReward());
        SDL_Log("========================================\n");
        SDL_Delay(1000);
        mGame->SetScene(new VictoryScene(mGame));
    }
    else
    {
        SDL_Log("       💀 DERROTA! 💀");
        SDL_Log("========================================");
        SDL_Log("O sapo foi derrotado...");
        SDL_Log("========================================\n");
        SDL_Delay(1000);
        mGame->SetScene(new GameOverScene(mGame));
    }
}

void CombatScene::Exit()
{
    SDL_Log("Exiting CombatScene");

    // Limpar recursos
    if (mCombatManager)
    {
        delete mCombatManager;
        mCombatManager = nullptr;
    }

    // IMPORTANTE: As cartas são deletadas com o Combatant
    // Não deletar cartas manualmente aqui

    if (mPlayer)
    {
        // Deletar cartas do player
        for (Card* card : mPlayer->GetDeck())
        {
            delete card;
        }
        delete mPlayer;
        mPlayer = nullptr;
    }

    if (mEnemy)
    {
        // Deletar cartas do enemy
        for (Card* card : mEnemy->GetDeck())
        {
            delete card;
        }
        delete mEnemy;
        mEnemy = nullptr;
    }
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