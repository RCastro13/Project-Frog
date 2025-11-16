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
#include "../Renderer/Font.h"
#include <SDL.h>
#include <SDL_log.h>
#include <algorithm>
#include <cstdio>

// ============================================
// CLASSE BASE: GameScene
// ============================================

GameScene::GameScene(Game* game)
    : mGame(game)
    , mStateTime(0.0f)
    , mFadeAlpha(1.0f)  // Começa com fade in (preto total)
    , mFadeTimer(0.0f)
    , mFadingIn(true)
{
}

GameScene::~GameScene()
{
}

void GameScene::UpdateFade(float deltaTime)
{
    const float FADE_DURATION = 0.5f; // 0.5 segundos para o fade

    if (mFadingIn)
    {
        // Fade in: preto (1.0) -> transparente (0.0)
        mFadeTimer += deltaTime;
        mFadeAlpha = 1.0f - (mFadeTimer / FADE_DURATION);

        if (mFadeAlpha <= 0.0f)
        {
            mFadeAlpha = 0.0f;
            mFadingIn = false; // Fade in completo
        }
    }
}

void GameScene::RenderFade()
{
    // Nota: DrawRect não suporta transparência (alpha) no momento
    // O bloqueio de inputs durante mFadeAlpha > 0 já resolve o problema
    // Uma implementação visual completa requer suporte a alpha no renderer
}

// ============================================
// MAIN MENU SCENE
// ============================================

MainMenuScene::MainMenuScene(Game* game)
    : GameScene(game)
    , mTitleTexture(nullptr)
    , mCommandsTexture(nullptr)
{
}

MainMenuScene::~MainMenuScene()
{
    if (mTitleTexture) {
        delete mTitleTexture;
    }
    if (mCommandsTexture) {
        delete mCommandsTexture;
    }
}

void MainMenuScene::Enter()
{
    SDL_Log("Entering MainMenuScene");
    mStateTime = 0.0f;

    // Atualizar título da janela para indicar cena ativa
    SDL_SetWindowTitle(mGame->GetWindow(), "Project Frog - MAIN MENU [1=Mapa 2=Combate ESC=Sair]");

    // Cor de fundo: Azul escuro (como o Mario original)
    mGame->GetRenderer()->SetClearColor(0.0f, 0.3f, 0.6f, 1.0f);

    // Criar texturas de texto
    if (mGame->GetFont()) {
        // Título do jogo
        mTitleTexture = mGame->GetFont()->RenderText(
            "PROJECT FROG",
            Vector3(1.0f, 1.0f, 0.0f),  // Amarelo
            48,
            500  // Limitar largura
        );

        // Comandos (com quebras de linha e wrap length reduzido)
        mCommandsTexture = mGame->GetFont()->RenderText(
            "[1] Mapa\n[2] Combate\n[ESC] Sair",
            Vector3(1.0f, 1.0f, 1.0f),  // Branco
            20,
            300  // Limitar largura para 300px
        );
    }
}

void MainMenuScene::Update(float deltaTime)
{
    mStateTime += deltaTime;

    // Atualizar fade in
    UpdateFade(deltaTime);

    // TODO (Kayque): Atualizar animações do menu
    // TODO (Kayque): Verificar seleção de botões
}

void MainMenuScene::ProcessInput(const Uint8* keyState)
{
    // Bloquear inputs durante o fade in
    if (mFadeAlpha > 0.0f)
        return;

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

void MainMenuScene::Render()
{
    // Renderizar título
    if (mTitleTexture) {
        mGame->GetRenderer()->DrawTexture(
            Vector2(320.0f, 120.0f),  // Centro superior
            Vector2(mTitleTexture->GetWidth(), mTitleTexture->GetHeight()),
            0.0f,
            Vector3(1.0f, 1.0f, 1.0f),
            mTitleTexture,
            Vector4::UnitRect,
            Vector2::Zero
        );
    }

    // Renderizar comandos
    if (mCommandsTexture) {
        mGame->GetRenderer()->DrawTexture(
            Vector2(320.0f, 260.0f),  // Centro da tela (ajustado para ficar melhor posicionado)
            Vector2(mCommandsTexture->GetWidth(), mCommandsTexture->GetHeight()),
            0.0f,
            Vector3(1.0f, 1.0f, 1.0f),
            mCommandsTexture,
            Vector4::UnitRect,
            Vector2::Zero
        );
    }

    // Renderizar fade
    RenderFade();
}

void MainMenuScene::Exit()
{
    SDL_Log("Exiting MainMenuScene");

    // Limpar texturas
    if (mTitleTexture) {
        delete mTitleTexture;
        mTitleTexture = nullptr;
    }
    if (mCommandsTexture) {
        delete mCommandsTexture;
        mCommandsTexture = nullptr;
    }
}

// ============================================
// MAP SCENE
// ============================================

MapScene::MapScene(Game* game)
    : GameScene(game)
    , mCurrentNode(nullptr)
    , mSelectedNode(nullptr)
    , mSelectedIndex(0)
    , mCameraPosition(Vector2::Zero)
{
}

MapScene::~MapScene()
{
    // Não deletar os nós - eles pertencem ao Game
    // Apenas limpar a lista de referências e ícones
    mMapNodes.clear();
    mNodeIcons.clear();
    mAvailableIcons.clear();
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

    // Carregar ícones disponíveis e atribuir aos nós (só se ainda não foram atribuídos)
    LoadAvailableIcons();
    if (mNodeIcons.empty() || mNodeIcons.size() != mMapNodes.size()) {
        AssignIconsToNodes();
    }

    // Inicializar posição da câmera para centralizar no nó inicial
    if (mCurrentNode) {
        Vector2 startPos = mCurrentNode->GetPosition();
        mCameraPosition = Vector2(startPos.x - 320.0f, 0.0f); // Centralizar horizontalmente (640/2)
    }

    SDL_Log("Mapa carregado com %d nós", (int)mMapNodes.size());
    SDL_Log("Nó atual: %d", mCurrentNode ? mCurrentNode->GetID() : -1);
}

void MapScene::Update(float deltaTime)
{
    mStateTime += deltaTime;

    // Atualizar fade in
    UpdateFade(deltaTime);

    // Atualizar scrolling com deltaTime correto
    const Uint8* keyState = SDL_GetKeyboardState(nullptr);
    static const float scrollSpeed = 300.0f; // pixels por segundo

    if (keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A]) {
        mCameraPosition.x -= scrollSpeed * deltaTime;
    }
    if (keyState[SDL_SCANCODE_RIGHT] || keyState[SDL_SCANCODE_D]) {
        mCameraPosition.x += scrollSpeed * deltaTime;
    }
}

void MapScene::ProcessInput(const Uint8* keyState)
{
    // Bloquear inputs durante o fade in
    if (mFadeAlpha > 0.0f)
        return;

    static bool upWasPressed = false;
    static bool downWasPressed = false;
    static bool enterWasPressed = false;

    // Scrolling é feito no Update() com deltaTime correto

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

void MapScene::RenderBackground()
{
    // Renderizar o mapa completo
    RenderMap();
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
    float size = 48.0f;  // Tamanho maior para os ícones

    // Determinar qual pasta de ícones usar baseado no estado
    std::string iconFolder = "White";
    if (node == mCurrentNode) {
        iconFolder = "Green";  // Nó atual = VERDE
    } else if (node->IsCompleted()) {
        iconFolder = "Red";    // Nós completados = VERMELHO
    } else {
        iconFolder = "White";  // Outros = BRANCO
    }

    // Obter o caminho do ícone baseado no tipo e pasta de cor
    std::string iconName = GetIconPathForNodeType(node->GetType());
    // Substituir "White" pela pasta correta
    size_t posWhite = iconName.find("/White/");
    if (posWhite != std::string::npos) {
        iconName.replace(posWhite, 7, "/" + iconFolder + "/");
    }

    // Carregar a textura do ícone com a cor correta
    Texture* iconTexture = mGame->GetRenderer()->GetTexture(iconName);

    // Se não encontrou na pasta de cor, tentar White como fallback
    if (!iconTexture) {
        std::string fallbackPath = GetIconPathForNodeType(node->GetType());
        iconTexture = mGame->GetRenderer()->GetTexture(fallbackPath);
    }

    // Determinar cor para tint (usar branco para manter cores originais dos ícones)
    Vector3 color = Vector3(1.0f, 1.0f, 1.0f);

    // Desenhar highlight para nó selecionado (borda pulsante)
    if (node == mSelectedNode && node != mCurrentNode) {
        // Efeito pulsante baseado no tempo
        float pulse = 0.5f + 0.3f * Math::Sin(mStateTime * 4.0f); // Pulsa entre 0.5 e 0.8
        float highlightSize = size + 12.0f * pulse;

        // Desenhar borda amarela ao redor do nó selecionado
        mGame->GetRenderer()->DrawRect(
            pos,
            Vector2(highlightSize, highlightSize),
            0.0f,
            Vector3(1.0f, 1.0f, 0.0f), // Amarelo para highlight
            mCameraPosition,
            RendererMode::TRIANGLES
        );
    }

    if (iconTexture) {
        // Desenhar ícone (usando posição da câmera para scrolling)
        mGame->GetRenderer()->DrawTexture(
            pos,
            Vector2(size, size),
            0.0f,
            color,
            iconTexture,
            Vector4::UnitRect,
            mCameraPosition,
            false,
            1.0f
        );
    } else {
        // Fallback: desenhar quadrado se não houver ícone
        mGame->GetRenderer()->DrawRect(
            pos,
            Vector2(size, size),
            0.0f,
            color,
            mCameraPosition,
            RendererMode::TRIANGLES
        );
    }

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
                mCameraPosition,
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

void MapScene::LoadAvailableIcons()
{
    // Não precisa mais carregar lista aleatória - cada tipo terá seu ícone específico
    // Esta função agora apenas inicializa o sistema de ícones
    SDL_Log("Sistema de ícones por tipo inicializado");
}

std::string MapScene::GetIconPathForNodeType(MapNodeType type)
{
    // Mapear cada tipo de nó para um ícone específico
    switch (type) {
        case MapNodeType::START:
            return "../Assets/Icons/White/icon_flag.png";
        case MapNodeType::COMBAT:
            return "../Assets/Icons/White/icon_sword.png";
        case MapNodeType::ELITE:
            return "../Assets/Icons/White/icon_skull.png";
        case MapNodeType::SHOP:
            return "../Assets/Icons/White/icon_coin.png";
        case MapNodeType::TREASURE:
            return "../Assets/Icons/White/icon_chest.png";
        case MapNodeType::REST:
            return "../Assets/Icons/White/icon_heart.png";
        case MapNodeType::BOSS:
            return "../Assets/Icons/White/icon_trophy.png";
        default:
            return "../Assets/Icons/White/icon_circle.png";
    }
}

void MapScene::AssignIconsToNodes()
{
    // Limpar ícones anteriores se houver
    mNodeIcons.clear();

    // Atribuir ícone específico baseado no tipo de cada nó
    for (MapNode* node : mMapNodes) {
        std::string iconPath = GetIconPathForNodeType(node->GetType());

        // Carregar a textura através do renderer
        Texture* iconTexture = mGame->GetRenderer()->GetTexture(iconPath);
        if (iconTexture) {
            mNodeIcons[node] = iconTexture;
        } else {
            SDL_Log("AVISO: Falha ao carregar ícone: %s", iconPath.c_str());
        }
    }

    SDL_Log("Atribuídos ícones a %d nós", (int)mNodeIcons.size());
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
    , mShowingCards(false)
    , mCardDisplayTimer(0.0f)
    , mDisplayPlayerCard(nullptr)
    , mDisplayEnemyCard(nullptr)
    , mPlayerWonLastTurn(false)
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

    // Atualizar fade in
    UpdateFade(deltaTime);

    // TODO (Facundo): Atualizar sistema de combate
    // TODO (Facundo): Atualizar animações
    // TODO (Facundo): Verificar condições de vitória/derrota
    if (!mCombatManager)
        return;

    // Gerenciar exibição das cartas no centro
    if (mShowingCards)
    {
        mCardDisplayTimer += deltaTime;

        // Após 2.5 segundos, finalizar exibição e continuar o combate
        if (mCardDisplayTimer >= 2.5f)
        {
            mShowingCards = false;
            mCardDisplayTimer = 0.0f;

            // Agora sim enviar ambas as cartas para o CombatManager
            if (mDisplayPlayerCard)
            {
                mCombatManager->PlayerSelectCard(mDisplayPlayerCard, mDisplayEnemyCard);
            }
        }

        // Não atualizar o combate enquanto mostra as cartas
        return;
    }

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

    // Se está mostrando as cartas no centro, renderizar elas
    if (mShowingCards) {
        RenderCardDisplay();
    }
    // Senão, renderizar as cartas do player normalmente
    else if (mPlayer && mCombatManager && mCombatManager->GetCurrentState() == CombatState::WAITING_FOR_PLAYER) {
        RenderCards();
    }
}

Vector3 CombatScene::GetCardColor(AttackType type)
{
    switch (type) {
        case AttackType::Fire:
            return Vector3(1.0f, 0.2f, 0.2f); // Vermelho
        case AttackType::Water:
            return Vector3(0.2f, 0.4f, 1.0f); // Azul
        case AttackType::Plant:
            return Vector3(0.2f, 1.0f, 0.2f); // Verde
        case AttackType::Neutral:
            return Vector3(0.1f, 0.1f, 0.1f); // Preto/cinza escuro
        default:
            return Vector3(0.5f, 0.5f, 0.5f); // Cinza
    }
}

void CombatScene::RenderCardDisplay()
{
    // Renderizar as duas cartas grandes no centro da tela
    float cardWidth = 120.0f;
    float cardHeight = 160.0f;
    float spacing = 40.0f;
    float centerY = 224.0f; // Centro vertical da tela

    // Posições das cartas
    float playerCardX = 320.0f - spacing - cardWidth / 2.0f;  // Esquerda do centro
    float enemyCardX = 320.0f + spacing + cardWidth / 2.0f;   // Direita do centro

    // Renderizar carta do Player (esquerda)
    if (mDisplayPlayerCard) {
        Vector2 playerCardPos(playerCardX, centerY);
        Vector3 playerColor = GetCardColor(mDisplayPlayerCard->GetType());

        // Brilho extra se venceu
        if (mPlayerWonLastTurn) {
            playerColor = playerColor * 1.3f; // Mais brilhante
        }

        // Carta
        mGame->GetRenderer()->DrawRect(
            playerCardPos,
            Vector2(cardWidth, cardHeight),
            0.0f,
            playerColor,
            Vector2::Zero,
            RendererMode::TRIANGLES
        );

        // Borda (dourada se venceu, branca se não)
        float borderThickness = 4.0f;
        Vector3 borderColor = mPlayerWonLastTurn ?
            Vector3(1.0f, 0.84f, 0.0f) :  // Dourado
            Vector3(1.0f, 1.0f, 1.0f);     // Branco

        // Top
        mGame->GetRenderer()->DrawRect(
            Vector2(playerCardX, centerY - (cardHeight + borderThickness)/2.0f),
            Vector2(cardWidth + borderThickness * 2, borderThickness),
            0.0f, borderColor, Vector2::Zero, RendererMode::TRIANGLES
        );
        // Bottom
        mGame->GetRenderer()->DrawRect(
            Vector2(playerCardX, centerY + (cardHeight + borderThickness)/2.0f),
            Vector2(cardWidth + borderThickness * 2, borderThickness),
            0.0f, borderColor, Vector2::Zero, RendererMode::TRIANGLES
        );
        // Left
        mGame->GetRenderer()->DrawRect(
            Vector2(playerCardX - (cardWidth + borderThickness)/2.0f, centerY),
            Vector2(borderThickness, cardHeight + borderThickness * 2),
            0.0f, borderColor, Vector2::Zero, RendererMode::TRIANGLES
        );
        // Right
        mGame->GetRenderer()->DrawRect(
            Vector2(playerCardX + (cardWidth + borderThickness)/2.0f, centerY),
            Vector2(borderThickness, cardHeight + borderThickness * 2),
            0.0f, borderColor, Vector2::Zero, RendererMode::TRIANGLES
        );

        // Renderizar valor de poder
        if (mGame->GetFont()) {
            char powerText[16];
            snprintf(powerText, sizeof(powerText), "%d", mDisplayPlayerCard->GetDamage());
            Texture* powerTexture = mGame->GetFont()->RenderText(
                powerText,
                Vector3(1.0f, 1.0f, 1.0f),  // Branco
                42
            );
            if (powerTexture) {
                mGame->GetRenderer()->DrawTexture(
                    Vector2(playerCardX, centerY),
                    Vector2(powerTexture->GetWidth(), powerTexture->GetHeight()),
                    0.0f,
                    Vector3(1.0f, 1.0f, 1.0f),
                    powerTexture,
                    Vector4::UnitRect,
                    Vector2::Zero
                );
                delete powerTexture;
            }
        }
    }

    // Renderizar carta do Enemy (direita)
    if (mDisplayEnemyCard) {
        Vector2 enemyCardPos(enemyCardX, centerY);
        Vector3 enemyColor = GetCardColor(mDisplayEnemyCard->GetType());

        // Brilho extra se venceu
        if (!mPlayerWonLastTurn) {
            enemyColor = enemyColor * 1.3f; // Mais brilhante
        }

        // Carta
        mGame->GetRenderer()->DrawRect(
            enemyCardPos,
            Vector2(cardWidth, cardHeight),
            0.0f,
            enemyColor,
            Vector2::Zero,
            RendererMode::TRIANGLES
        );

        // Borda (dourada se venceu, branca se não)
        float borderThickness = 4.0f;
        Vector3 borderColor = !mPlayerWonLastTurn ?
            Vector3(1.0f, 0.84f, 0.0f) :  // Dourado
            Vector3(1.0f, 1.0f, 1.0f);     // Branco

        // Top
        mGame->GetRenderer()->DrawRect(
            Vector2(enemyCardX, centerY - (cardHeight + borderThickness)/2.0f),
            Vector2(cardWidth + borderThickness * 2, borderThickness),
            0.0f, borderColor, Vector2::Zero, RendererMode::TRIANGLES
        );
        // Bottom
        mGame->GetRenderer()->DrawRect(
            Vector2(enemyCardX, centerY + (cardHeight + borderThickness)/2.0f),
            Vector2(cardWidth + borderThickness * 2, borderThickness),
            0.0f, borderColor, Vector2::Zero, RendererMode::TRIANGLES
        );
        // Left
        mGame->GetRenderer()->DrawRect(
            Vector2(enemyCardX - (cardWidth + borderThickness)/2.0f, centerY),
            Vector2(borderThickness, cardHeight + borderThickness * 2),
            0.0f, borderColor, Vector2::Zero, RendererMode::TRIANGLES
        );
        // Right
        mGame->GetRenderer()->DrawRect(
            Vector2(enemyCardX + (cardWidth + borderThickness)/2.0f, centerY),
            Vector2(borderThickness, cardHeight + borderThickness * 2),
            0.0f, borderColor, Vector2::Zero, RendererMode::TRIANGLES
        );

        // Renderizar valor de poder
        if (mGame->GetFont()) {
            char powerText[16];
            snprintf(powerText, sizeof(powerText), "%d", mDisplayEnemyCard->GetDamage());
            Texture* powerTexture = mGame->GetFont()->RenderText(
                powerText,
                Vector3(1.0f, 1.0f, 1.0f),  // Branco
                42
            );
            if (powerTexture) {
                mGame->GetRenderer()->DrawTexture(
                    Vector2(enemyCardX, centerY),
                    Vector2(powerTexture->GetWidth(), powerTexture->GetHeight()),
                    0.0f,
                    Vector3(1.0f, 1.0f, 1.0f),
                    powerTexture,
                    Vector4::UnitRect,
                    Vector2::Zero
                );
                delete powerTexture;
            }
        }
    }
}

void CombatScene::RenderCards()
{
    // Dimensões e posicionamento das cartas
    float cardWidth = 80.0f;
    float cardHeight = 110.0f;
    float cardSpacing = 20.0f;
    float baseY = 380.0f; // Parte inferior da tela (448 - margem)
    float selectedOffset = -20.0f; // Offset para cima quando selecionada

    // Calcular posição inicial X para centralizar as 4 cartas
    float totalWidth = (cardWidth * 4) + (cardSpacing * 3);
    float startX = (640.0f - totalWidth) / 2.0f + cardWidth / 2.0f;

    for (int i = 0; i < 4; i++) {
        Card* card = mPlayer->GetDeck()[i];

        // Calcular posição da carta
        float cardX = startX + (i * (cardWidth + cardSpacing));
        float cardY = baseY;

        // Carta selecionada fica mais acima
        if (i == mSelectedCardIndex) {
            cardY += selectedOffset;
        }

        Vector2 cardPos(cardX, cardY);

        // Determinar cor da carta
        Vector3 cardColor;
        if (!card->IsAvailable()) {
            // Carta em cooldown = cinza
            cardColor = Vector3(0.3f, 0.3f, 0.3f);
        } else {
            // Usar função helper para obter cor
            cardColor = GetCardColor(card->GetType());
        }

        // Renderizar carta
        mGame->GetRenderer()->DrawRect(
            cardPos,
            Vector2(cardWidth, cardHeight),
            0.0f,
            cardColor,
            Vector2::Zero,
            RendererMode::TRIANGLES
        );

        // Renderizar borda da carta (mais grossa se selecionada)
        float borderThickness = (i == mSelectedCardIndex) ? 3.0f : 2.0f;
        Vector3 borderColor = (i == mSelectedCardIndex) ?
            Vector3(1.0f, 1.0f, 0.0f) : // Amarelo se selecionada
            Vector3(1.0f, 1.0f, 1.0f);  // Branco se não selecionada

        // Top
        mGame->GetRenderer()->DrawRect(
            Vector2(cardX, cardY - (cardHeight + borderThickness)/2.0f),
            Vector2(cardWidth + borderThickness * 2, borderThickness),
            0.0f, borderColor, Vector2::Zero, RendererMode::TRIANGLES
        );
        // Bottom
        mGame->GetRenderer()->DrawRect(
            Vector2(cardX, cardY + (cardHeight + borderThickness)/2.0f),
            Vector2(cardWidth + borderThickness * 2, borderThickness),
            0.0f, borderColor, Vector2::Zero, RendererMode::TRIANGLES
        );
        // Left
        mGame->GetRenderer()->DrawRect(
            Vector2(cardX - (cardWidth + borderThickness)/2.0f, cardY),
            Vector2(borderThickness, cardHeight + borderThickness * 2),
            0.0f, borderColor, Vector2::Zero, RendererMode::TRIANGLES
        );
        // Right
        mGame->GetRenderer()->DrawRect(
            Vector2(cardX + (cardWidth + borderThickness)/2.0f, cardY),
            Vector2(borderThickness, cardHeight + borderThickness * 2),
            0.0f, borderColor, Vector2::Zero, RendererMode::TRIANGLES
        );

        // Renderizar valor de poder (centralizado na carta)
        if (mGame->GetFont() && card->IsAvailable()) {
            char powerText[16];
            snprintf(powerText, sizeof(powerText), "%d", card->GetDamage());
            Texture* powerTexture = mGame->GetFont()->RenderText(
                powerText,
                Vector3(1.0f, 1.0f, 1.0f),  // Branco
                28
            );
            if (powerTexture) {
                mGame->GetRenderer()->DrawTexture(
                    Vector2(cardX, cardY),
                    Vector2(powerTexture->GetWidth(), powerTexture->GetHeight()),
                    0.0f,
                    Vector3(1.0f, 1.0f, 1.0f),
                    powerTexture,
                    Vector4::UnitRect,
                    Vector2::Zero
                );
                delete powerTexture;
            }
        }
    }
}

void CombatScene::ProcessInput(const Uint8* keyState)
{
    if (!mCombatManager)
        return;

    // Bloquear inputs durante o fade in
    if (mFadeAlpha > 0.0f)
        return;

    // Bloquear inputs enquanto mostra as cartas no centro
    if (mShowingCards)
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

            // Ao invés de chamar PlayerSelectCard imediatamente,
            // ativar o modo de exibição de cartas
            mDisplayPlayerCard = selectedCard;
            mDisplayEnemyCard = mEnemy->SelectCard(); // Enemy seleciona sua carta
            mShowingCards = true;
            mCardDisplayTimer = 0.0f;

            // Determinar quem venceu usando a mesma lógica de Card::Fight
            if (mDisplayEnemyCard) {
                // Primeiro verificar vantagem de tipo
                bool playerHasAdvantage = mDisplayPlayerCard->HasTypeAdvantageOver(mDisplayEnemyCard);
                bool enemyHasAdvantage = mDisplayEnemyCard->HasTypeAdvantageOver(mDisplayPlayerCard);

                if (playerHasAdvantage) {
                    mPlayerWonLastTurn = true;
                } else if (enemyHasAdvantage) {
                    mPlayerWonLastTurn = false;
                } else {
                    // Sem vantagem de tipo, comparar dano
                    mPlayerWonLastTurn = (mDisplayPlayerCard->GetDamage() >= mDisplayEnemyCard->GetDamage());
                }
            } else {
                mPlayerWonLastTurn = true;
            }

            SDL_Log("Enemy usou: %s (Tipo: %s, Dano: %d)",
                    mDisplayEnemyCard ? mDisplayEnemyCard->GetName().c_str() : "None",
                    mDisplayEnemyCard ? GetTypeName(mDisplayEnemyCard->GetType()) : "None",
                    mDisplayEnemyCard ? mDisplayEnemyCard->GetDamage() : 0);

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