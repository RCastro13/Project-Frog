//
// Created by Mi PC on 05/11/2025.
//

#include "GameScene.h"
#include "../Game.h"
#include "../Combat/CombatManager.h"
#include "../Combat/Card.h"
#include "../Actors/Player.h"
#include "../Actors/Enemy.h"
#include "../Actors/FrogActor.h"
#include "../Actors/BearActor.h"
#include "../Map/MapNode.h"
#include "../Map/MapGenerator.h"
#include "../Random.h"
#include "../Renderer/Texture.h"
#include "../Renderer/Renderer.h"
#include <SDL.h>
#include <SDL_log.h>
#include <algorithm>

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
    , mCurrentNode(nullptr)
    , mSelectedNode(nullptr)
    , mSelectedIndex(0)
{
}

MapScene::~MapScene()
{
    // Não deletar os nós - eles pertencem ao Game
    // Apenas limpar a lista de referências
    mMapNodes.clear();
}

void MapScene::Enter()
{
    SDL_Log("Entering MapScene");
    mStateTime = 0.0f;

    // Atualizar título da janela
    SDL_SetWindowTitle(mGame->GetWindow(), "Project Frog - MAPA [↑↓=Navegar ENTER=Selecionar]");

    // Cor de fundo: Verde (tema natureza/pântano)
    mGame->GetRenderer()->SetClearColor(0.2f, 0.5f, 0.3f, 1.0f);

    // Obter mapa do Game (se já existe) ou gerar novo
    if (mGame->GetMapNodes().empty()) {
        // Gerar novo mapa
        SDL_Log("Gerando novo mapa...");
        mMapNodes = MapGenerator::Generate();
        mGame->SetMapNodes(mMapNodes);

        // Definir nó inicial como atual
        if (!mMapNodes.empty()) {
            mCurrentNode = mMapNodes[0];
            mGame->SetCurrentMapNode(mCurrentNode);
        }
    } else {
        // Usar mapa existente
        mMapNodes = mGame->GetMapNodes();
        mCurrentNode = mGame->GetCurrentMapNode();
    }

    // Selecionar primeiro nó acessível
    std::vector<MapNode*> accessible = GetAccessibleNodes();
    if (!accessible.empty()) {
        mSelectedNode = accessible[0];
        mSelectedIndex = 0;
    }

    SDL_Log("Mapa carregado com %d nós", (int)mMapNodes.size());
    SDL_Log("Nó atual: %d", mCurrentNode ? mCurrentNode->GetID() : -1);
}

void MapScene::Update(float deltaTime)
{
    mStateTime += deltaTime;

    // Renderizar o mapa (chamado pelo renderer do Game)
    RenderMap();
}

void MapScene::ProcessInput(const Uint8* keyState)
{
    static bool upWasPressed = false;
    static bool downWasPressed = false;
    static bool enterWasPressed = false;

    // Navegar para cima
    if (keyState[SDL_SCANCODE_UP] && !upWasPressed) {
        SelectPreviousAccessibleNode();
        upWasPressed = true;
    } else if (!keyState[SDL_SCANCODE_UP]) {
        upWasPressed = false;
    }

    // Navegar para baixo
    if (keyState[SDL_SCANCODE_DOWN] && !downWasPressed) {
        SelectNextAccessibleNode();
        downWasPressed = true;
    } else if (!keyState[SDL_SCANCODE_DOWN]) {
        downWasPressed = false;
    }

    // Confirmar seleção
    if (keyState[SDL_SCANCODE_RETURN] && !enterWasPressed) {
        ConfirmSelection();
        enterWasPressed = true;
    } else if (!keyState[SDL_SCANCODE_RETURN]) {
        enterWasPressed = false;
    }
}

void MapScene::Exit()
{
    SDL_Log("Exiting MapScene");
}

void MapScene::SetCurrentNode(MapNode* node)
{
    if (mCurrentNode) {
        mCurrentNode->SetCurrent(false);
    }
    mCurrentNode = node;
    if (mCurrentNode) {
        mCurrentNode->SetCurrent(true);
    }
}

void MapScene::RenderMap()
{
    // Renderizar conexões primeiro (para ficarem atrás dos nós)
    RenderConnections();

    // Renderizar todos os nós
    for (MapNode* node : mMapNodes) {
        RenderNode(node);
    }
}

void MapScene::RenderNode(MapNode* node)
{
    Vector2 pos = node->GetPosition();
    float size = 32.0f;

    // Determinar cor baseada no estado
    Vector3 color = Vector3(0.5f, 0.5f, 0.5f); // Cinza (bloqueado)

    if (node->IsCompleted()) {
        color = Vector3(0.0f, 1.0f, 0.0f); // Verde (completo)
    } else if (node == mCurrentNode) {
        color = Vector3(0.0f, 0.5f, 1.0f); // Azul (atual)
    } else if (node == mSelectedNode) {
        color = Vector3(1.0f, 1.0f, 0.0f); // Amarelo (selecionado)
    } else if (node->IsAccessible()) {
        color = Vector3(1.0f, 1.0f, 1.0f); // Branco (acessível)
    }

    // Desenhar quadrado representando o nó
    mGame->GetRenderer()->DrawRect(
        pos,
        Vector2(size, size),
        0.0f,
        color,
        Vector2::Zero,
        RendererMode::TRIANGLES
    );

    // Log informativo quando renderizar pela primeira vez
    static bool firstRender = true;
    if (firstRender && node == mCurrentNode) {
        SDL_Log("Renderizando nó %d (%s) na posição (%.1f, %.1f)",
                node->GetID(), GetNodeTypeName(node->GetType()),
                pos.x, pos.y);
        firstRender = false;
    }
}

void MapScene::RenderConnections()
{
    // Desenhar linhas conectando os nós
    for (MapNode* node : mMapNodes) {
        Vector2 startPos = node->GetPosition();

        for (MapNode* child : node->GetChildren()) {
            Vector2 endPos = child->GetPosition();

            // Cor da linha baseada no estado
            Vector3 lineColor = Vector3(0.3f, 0.3f, 0.3f); // Cinza escuro padrão
            if (node->IsAccessible() || child->IsAccessible()) {
                lineColor = Vector3(0.6f, 0.6f, 0.6f); // Cinza claro
            }
            if (node->IsCompleted()) {
                lineColor = Vector3(0.0f, 0.7f, 0.0f); // Verde
            }

            // Desenhar linha (simplificado - apenas um retângulo fino)
            Vector2 diff = endPos - startPos;
            float length = diff.Length();
            float angle = Math::Atan2(diff.y, diff.x);
            Vector2 midpoint = startPos + diff * 0.5f;

            mGame->GetRenderer()->DrawRect(
                midpoint,
                Vector2(length, 2.0f),
                angle,
                lineColor,
                Vector2::Zero,
                RendererMode::TRIANGLES
            );
        }
    }
}

void MapScene::SelectNextAccessibleNode()
{
    std::vector<MapNode*> accessible = GetAccessibleNodes();
    if (accessible.empty()) return;

    mSelectedIndex = (mSelectedIndex + 1) % accessible.size();
    mSelectedNode = accessible[mSelectedIndex];

    SDL_Log("Nó selecionado: %d (%s)",
            mSelectedNode->GetID(),
            GetNodeTypeName(mSelectedNode->GetType()));
}

void MapScene::SelectPreviousAccessibleNode()
{
    std::vector<MapNode*> accessible = GetAccessibleNodes();
    if (accessible.empty()) return;

    mSelectedIndex = (mSelectedIndex - 1 + accessible.size()) % accessible.size();
    mSelectedNode = accessible[mSelectedIndex];

    SDL_Log("Nó selecionado: %d (%s)",
            mSelectedNode->GetID(),
            GetNodeTypeName(mSelectedNode->GetType()));
}

void MapScene::ConfirmSelection()
{
    if (!mSelectedNode || !CanSelectNode(mSelectedNode)) {
        SDL_Log("Não é possível selecionar este nó");
        return;
    }

    SDL_Log("Confirmando seleção do nó %d (%s)",
            mSelectedNode->GetID(),
            GetNodeTypeName(mSelectedNode->GetType()));

    // Marcar nó atual como completo
    if (mCurrentNode) {
        mCurrentNode->SetCompleted(true);
        mCurrentNode->SetCurrent(false);
    }

    // Atualizar nó atual
    SetCurrentNode(mSelectedNode);
    mGame->SetCurrentMapNode(mSelectedNode);

    // Tornar filhos acessíveis
    for (MapNode* child : mSelectedNode->GetChildren()) {
        child->SetAccessible(true);
    }

    // Transição para a cena apropriada
    switch (mSelectedNode->GetType()) {
        case MapNodeType::START:
        case MapNodeType::COMBAT:
            SDL_Log("==> Iniciando combate normal");
            mGame->SetScene(new CombatScene(mGame));
            break;

        case MapNodeType::ELITE:
            SDL_Log("==> Iniciando combate elite");
            mGame->SetScene(new CombatScene(mGame)); // Por enquanto igual ao normal
            break;

        case MapNodeType::BOSS:
            SDL_Log("==> Iniciando combate contra o BOSS!");
            mGame->SetScene(new CombatScene(mGame));
            break;

        case MapNodeType::SHOP:
            SDL_Log("==> Entrando na loja (não implementada ainda)");
            // TODO: mGame->SetScene(new ShopScene(mGame));
            // Por enquanto, volta para o mapa
            mGame->SetScene(new MapScene(mGame));
            break;

        case MapNodeType::TREASURE:
            SDL_Log("==> Abrindo baú de tesouro (não implementado ainda)");
            // TODO: mGame->SetScene(new TreasureScene(mGame));
            mGame->SetScene(new MapScene(mGame));
            break;

        case MapNodeType::REST:
            SDL_Log("==> Descansando (não implementado ainda)");
            // TODO: mGame->SetScene(new RestScene(mGame));
            mGame->SetScene(new MapScene(mGame));
            break;
    }
}

bool MapScene::CanSelectNode(MapNode* node)
{
    if (!node) return false;

    // Só pode selecionar nós acessíveis que não estão completos
    return node->IsAccessible() && !node->IsCompleted();
}

std::vector<MapNode*> MapScene::GetAccessibleNodes()
{
    std::vector<MapNode*> accessible;

    for (MapNode* node : mMapNodes) {
        if (CanSelectNode(node)) {
            accessible.push_back(node);
        }
    }

    return accessible;
}

const char* MapScene::GetNodeTypeName(MapNodeType type)
{
    switch (type) {
        case MapNodeType::START:    return "START";
        case MapNodeType::COMBAT:   return "COMBAT";
        case MapNodeType::ELITE:    return "ELITE";
        case MapNodeType::SHOP:     return "SHOP";
        case MapNodeType::TREASURE: return "TREASURE";
        case MapNodeType::REST:     return "REST";
        case MapNodeType::BOSS:     return "BOSS";
        default:                    return "UNKNOWN";
    }
}

// ============================================
// COMBAT SCENE
// ============================================

CombatScene::CombatScene(Game* game)
    : GameScene(game)
    , mCombatManager(nullptr)
    , mPlayer(nullptr)
    , mEnemy(nullptr)
    , mFrogActor(nullptr)
    , mBearActor(nullptr)
    , mBackgroundTexture(nullptr)
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

    // Selecionar background aleatório
    const char* backgroundFiles[] = {
        "../Assets/Background/Combat/Background_camada_0_01.png",
        "../Assets/Background/Combat/Background_camada_0_05.png",
        "../Assets/Background/Combat/Background_camada_0_06.png",
        "../Assets/Background/Combat/Background_camada_0_07.png"
    };

    int randomIndex = Random::GetIntRange(0, 3);
    mBackgroundTexture = mGame->GetRenderer()->GetTexture(backgroundFiles[randomIndex]);

    if (mBackgroundTexture) {
        SDL_Log("Background de combate carregado: %s", backgroundFiles[randomIndex]);
    } else {
        SDL_Log("ERRO: Falha ao carregar background de combate");
    }

    // Cor de fundo: Preto (será coberto pelo background)
    mGame->GetRenderer()->SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Criar combatentes e iniciar combate
    CreateTestCombatants();

    // Criar atores visuais
    CreateVisualActors();

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

void CombatScene::CreateVisualActors()
{
    // Criar sapo (player) à esquerda
    mFrogActor = new FrogActor(mGame);
    mFrogActor->SetPosition(Vector2(150.0f, 224.0f)); // Esquerda, centro vertical
    mFrogActor->SetScale(Vector2(1.0f, 1.0f));

    SDL_Log("FrogActor criado na posição (%.1f, %.1f)",
            mFrogActor->GetPosition().x, mFrogActor->GetPosition().y);

    // Criar urso (inimigo) à direita
    mBearActor = new BearActor(mGame);
    mBearActor->SetPosition(Vector2(490.0f, 224.0f)); // Direita, centro vertical
    mBearActor->SetScale(Vector2(-1.0f, 1.0f)); // Inverter horizontalmente (olhar para a esquerda)

    SDL_Log("BearActor criado na posição (%.1f, %.1f)",
            mBearActor->GetPosition().x, mBearActor->GetPosition().y);
}

void CombatScene::Update(float deltaTime)
{
    mStateTime += deltaTime;

    // TODO (Facundo): Atualizar sistema de combate
    // TODO (Facundo): Atualizar animações
    // TODO (Facundo): Verificar condições de vitória/derrota
    if (!mCombatManager)
        return;

    // Guardar estado anterior para detectar mudanças
    static CombatState previousState = CombatState::WAITING_FOR_PLAYER;
    CombatState currentState = mCombatManager->GetCurrentState();

    // Detectar quando começa a resolver combate (tocar animações de ataque)
    if (previousState != CombatState::RESOLVING_COMBAT &&
        currentState == CombatState::RESOLVING_COMBAT)
    {
        // Ambos atacam ao mesmo tempo
        if (mFrogActor) mFrogActor->PlayAttack();
        if (mBearActor) mBearActor->PlayAttack();
    }

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

        // Voltar para idle após o turno
        if (mFrogActor) mFrogActor->PlayIdle();
        if (mBearActor) mBearActor->PlayIdle();
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

void CombatScene::RenderBackground()
{
    // Renderizar background primeiro (se carregado)
    if (mBackgroundTexture) {
        // Renderizar background na tela inteira (640x448)
        mGame->GetRenderer()->DrawTexture(
            Vector2(320.0f, 224.0f), // Centro da tela
            Vector2(640.0f, 448.0f), // Tamanho da tela
            0.0f,                     // Sem rotação
            Vector3(1.0f, 1.0f, 1.0f), // Cor branca (sem tint)
            mBackgroundTexture,
            Vector4::UnitRect,        // Textura completa
            Vector2::Zero             // Sem offset de câmera
        );
    }
}

void CombatScene::Render()
{
    // Renderizar UI do combate (barras de HP, etc)
    RenderCombatUI();
}

void CombatScene::RenderCombatUI()
{
    // TODO (Rubens): Adicionar renderização visual com sprites/texto na tela
    // Por enquanto, mantemos apenas os logs essenciais

    // Renderizar barras de HP
    if (mPlayer && mEnemy) {
        // Barra do Player (em cima do FrogActor)
        Vector2 playerHPPos = Vector2(150.0f, 160.0f);
        RenderHealthBar(playerHPPos, mPlayer->GetHealth(), 20, false);

        // Barra do Enemy (em cima do BearActor)
        Vector2 enemyHPPos = Vector2(490.0f, 160.0f);
        RenderHealthBar(enemyHPPos, mEnemy->GetHealth(), 15, true);
    }
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

void CombatScene::RenderHealthBar(Vector2 position, int currentHP, int maxHP, bool isEnemy)
{
    // Dimensões da barra (maior para ser mais visível)
    float barWidth = 120.0f;
    float barHeight = 12.0f;

    // Calcular porcentagem de HP
    float hpPercent = (float)currentHP / (float)maxHP;
    float currentBarWidth = barWidth * hpPercent;

    // Cor da barra baseada na porcentagem
    Vector3 hpColor;
    if (hpPercent > 0.6f) {
        hpColor = Vector3(0.2f, 1.0f, 0.2f); // Verde brilhante
    } else if (hpPercent > 0.3f) {
        hpColor = Vector3(1.0f, 1.0f, 0.0f); // Amarelo
    } else {
        hpColor = Vector3(1.0f, 0.2f, 0.2f); // Vermelho brilhante
    }

    // Fundo da barra (cinza escuro/preto)
    mGame->GetRenderer()->DrawRect(
        position,
        Vector2(barWidth, barHeight),
        0.0f,
        Vector3(0.15f, 0.15f, 0.15f),
        Vector2::Zero,
        RendererMode::TRIANGLES
    );

    // Barra de HP preenchida (ajustar posição X para alinhar à esquerda)
    if (currentHP > 0) {
        float xOffset = -(barWidth - currentBarWidth) / 2.0f;
        mGame->GetRenderer()->DrawRect(
            Vector2(position.x + xOffset, position.y),
            Vector2(currentBarWidth, barHeight),
            0.0f,
            hpColor,
            Vector2::Zero,
            RendererMode::TRIANGLES
        );
    }

    // Borda branca (mais grossa)
    float borderThickness = 2.0f;
    Vector3 borderColor = Vector3(1.0f, 1.0f, 1.0f);

    // Top
    mGame->GetRenderer()->DrawRect(
        Vector2(position.x, position.y - (barHeight + borderThickness)/2.0f),
        Vector2(barWidth + borderThickness * 2, borderThickness),
        0.0f, borderColor, Vector2::Zero, RendererMode::TRIANGLES
    );
    // Bottom
    mGame->GetRenderer()->DrawRect(
        Vector2(position.x, position.y + (barHeight + borderThickness)/2.0f),
        Vector2(barWidth + borderThickness * 2, borderThickness),
        0.0f, borderColor, Vector2::Zero, RendererMode::TRIANGLES
    );
    // Left
    mGame->GetRenderer()->DrawRect(
        Vector2(position.x - (barWidth + borderThickness)/2.0f, position.y),
        Vector2(borderThickness, barHeight + borderThickness * 2),
        0.0f, borderColor, Vector2::Zero, RendererMode::TRIANGLES
    );
    // Right
    mGame->GetRenderer()->DrawRect(
        Vector2(position.x + (barWidth + borderThickness)/2.0f, position.y),
        Vector2(borderThickness, barHeight + borderThickness * 2),
        0.0f, borderColor, Vector2::Zero, RendererMode::TRIANGLES
    );
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

        // Verificar se era o boss
        MapNode* currentNode = mGame->GetCurrentMapNode();
        if (currentNode && currentNode->GetType() == MapNodeType::BOSS) {
            SDL_Log("🏆 VOCÊ DERROTOU O BOSS! JOGO COMPLETO! 🏆");
            mGame->SetScene(new VictoryScene(mGame));
        } else {
            // Voltar para o mapa
            SDL_Log("Voltando ao mapa...");
            mGame->SetScene(new MapScene(mGame));
        }
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

    // TODO (Facundo): Limpar recursos do combate
    // TODO (Facundo): Deletar inimigo
    // TODO (Facundo): Parar música de combate
    // Limpar atores visuais
    if (mFrogActor)
    {
        mFrogActor->SetState(ActorState::Destroy);
        mFrogActor = nullptr;
    }

    if (mBearActor)
    {
        mBearActor->SetState(ActorState::Destroy);
        mBearActor = nullptr;
    }

    // Limpar background (o Renderer gerencia a memória da textura)
    mBackgroundTexture = nullptr;

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