//
// Created by Mi PC on 05/11/2025.
//

#include "GameScene.h"
#include "../Game.h"
#include "../Combat/CombatManager.h"
#include "../Combat/CombatRenderer.h"
#include "../Combat/CombatConstants.h"
#include "../Combat/Card.h"
#include "../Actors/Player.h"
#include "../Actors/Enemy.h"
#include "../Actors/FrogActor.h"
#include "../Actors/BearActor.h"
#include "../Actors/MagicProjectileActor.h"
#include "../Map/MapNode.h"
#include "../Map/MapGenerator.h"
#include "../Random.h"
#include "../Renderer/Texture.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/Font.h"
#include <SDL.h>
#include <SDL_log.h>
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
    , mBackgroundTexture(nullptr)
    , mCameraPosition(Vector2::Zero)
    , mMinCameraX(-200.0f)
    , mMaxCameraX( Game::WINDOW_WIDTH + 200.0f)
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

    // Carregar background do mapa
    mBackgroundTexture = mGame->GetRenderer()->GetTexture("../Assets/Background/Map/mapa.png");
    if (mBackgroundTexture) {
        SDL_Log("Background do mapa carregado");
    } else {
        SDL_Log("AVISO: Falha ao carregar background do mapa");
    }

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

    // Tornar apenas os filhos do nó inicial acessíveis (se houver nó atual)
    if (mCurrentNode) {
        // Marcar todos os nós como inacessíveis primeiro
        for (MapNode* node : mMapNodes) {
            if (node != mCurrentNode) {
                node->SetAccessible(false);
            }
        }
        // Tornar apenas os filhos do nó inicial acessíveis
        for (MapNode* child : mCurrentNode->GetChildren()) {
            child->SetAccessible(true);
        }
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

    // Inicializar posição da câmera para colocar o nó inicial mais à esquerda
    if (mCurrentNode) {
        Vector2 startPos = mCurrentNode->GetPosition();
        mCameraPosition = Vector2(startPos.x - 100.0f, 0.0f); // Nó inicial mais à esquerda (100px da borda)
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

    // Limitar câmera aos bounds do mapa
    if (mCameraPosition.x < mMinCameraX) {
        mCameraPosition.x = mMinCameraX;
    }
    if (mCameraPosition.x > mMaxCameraX) {
        mCameraPosition.x = mMaxCameraX;
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

    // Limpar referência ao background (Renderer gerencia a memória)
    mBackgroundTexture = nullptr;
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
    // Renderizar background scrollável
    if (mBackgroundTexture) {
        // Largura do mapa: 1280 + margens (200px de cada lado = 400px)
        float backgroundWidth = 1280.0f + 400.0f;
        float backgroundHeight = 448.0f;

        // Centro do background no mundo
        float centerX = backgroundWidth / 2.0f - 200.0f;
        float centerY = backgroundHeight / 2.0f;

        mGame->GetRenderer()->DrawTexture(
            Vector2(centerX, centerY),       // Centro do background no mundo
            Vector2(backgroundWidth, backgroundHeight),
            0.0f,
            Vector3(1.0f, 1.0f, 1.0f),
            mBackgroundTexture,
            Vector4::UnitRect,
            mCameraPosition,                 // Offset da câmera para scrolling
            false,
            1.0f
        );
    }

    // Renderizar o mapa completo por cima do background
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

    // Guardar o nó selecionado antes de fazer mudanças
    MapNode* nodeToMoveTo = mSelectedNode;

    SDL_Log("Confirmando seleção do nó %d (%s)",
            nodeToMoveTo->GetID(),
            GetNodeTypeName(nodeToMoveTo->GetType()));

    // Marcar nó atual como completo
    if (mCurrentNode) {
        mCurrentNode->SetCompleted(true);
        mCurrentNode->SetCurrent(false);
    }

    // Atualizar nó atual
    SetCurrentNode(nodeToMoveTo);
    mGame->SetCurrentMapNode(nodeToMoveTo);

    // Prevenir retrocesso: marcar todos os nós como inacessíveis primeiro
    for (MapNode* node : mMapNodes) {
        if (node != nodeToMoveTo) {
            node->SetAccessible(false);
        }
    }

    // Tornar apenas os filhos do novo nó atual acessíveis
    for (MapNode* child : nodeToMoveTo->GetChildren()) {
        child->SetAccessible(true);
    }

    // Atualizar seleção para o primeiro nó acessível (se houver)
    std::vector<MapNode*> accessible = GetAccessibleNodes();
    if (!accessible.empty()) {
        mSelectedNode = accessible[0];
        mSelectedIndex = 0;
    } else {
        mSelectedNode = nullptr;
        mSelectedIndex = 0;
    }

    // Transição para a cena apropriada (usar o nó que foi selecionado, não o novo selecionado)
    switch (nodeToMoveTo->GetType()) {
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
    , mCombatRenderer(nullptr)
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
    , mProjectile(nullptr)
    , mShowingProjectile(false)
    , mProjectileTimer(0.0f)
{
}

CombatScene::~CombatScene()
{
}

void CombatScene::Enter()
{
    mStateTime = 0.0f;

    // Atualizar título da janela
    SDL_SetWindowTitle(mGame->GetWindow(), "Project Frog - COMBATE [Setas=Selecionar Enter=Confirmar]");

    // Selecionar background aleatório
    const char* backgroundFiles[] = {
        "../Assets/Background/Combat/pedras.png",
        "../Assets/Background/Combat/floresta.png",
        "../Assets/Background/Combat/pantano.jpeg",
    };

    int randomIndex = Random::GetIntRange(0, 0);  // Apenas 1 background disponível
    mBackgroundTexture = mGame->GetRenderer()->GetTexture(backgroundFiles[randomIndex]);

    if (mBackgroundTexture) {
        SDL_Log("Background de combate carregado: %s", backgroundFiles[randomIndex]);
    } else {
        SDL_Log("ERRO: Falha ao carregar background de combate");
    }

    mGame->GetRenderer()->SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Criar sistema de renderização
    mCombatRenderer = new CombatRenderer(mGame);

    // Criar combatentes e iniciar combate
    CreateTestCombatants();

    // Criar atores visuais
    CreateVisualActors();

    // Carregar texturas das cartas
    LoadCardTextures();

    // Carregar ícone de cooldown
    mTimeIconTexture = mGame->GetRenderer()->GetTexture("../Assets/Icons/White/icon_time.png");
    if (mTimeIconTexture) {
        SDL_Log("Ícone de cooldown carregado");
    }

    // Carregar moldura de vencedor
    mWinnerFrameTexture = mGame->GetRenderer()->GetTexture("../Assets/Card/winner-frame.png");
    if (mWinnerFrameTexture) {
        SDL_Log("Moldura de vencedor carregada");
    }
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

void CombatScene::LoadCardTextures()
{
    // Carregar texturas active (quando carta está disponível)
    mCardTexturesActive[AttackType::Fire] =
        mGame->GetRenderer()->GetTexture("../Assets/Cards/fire-active.png");
    mCardTexturesActive[AttackType::Water] =
        mGame->GetRenderer()->GetTexture("../Assets/Cards/water-active.png");
    mCardTexturesActive[AttackType::Plant] =
        mGame->GetRenderer()->GetTexture("../Assets/Cards/plant-active.png");
    mCardTexturesActive[AttackType::Neutral] =
        mGame->GetRenderer()->GetTexture("../Assets/Cards/neutral-active.png");

    // Carregar texturas cooldown (quando carta está em cooldown)
    mCardTexturesCooldown[AttackType::Fire] =
        mGame->GetRenderer()->GetTexture("../Assets/Cards/fire-cooldown.png");
    mCardTexturesCooldown[AttackType::Water] =
        mGame->GetRenderer()->GetTexture("../Assets/Cards/water-cooldown.png");
    mCardTexturesCooldown[AttackType::Plant] =
        mGame->GetRenderer()->GetTexture("../Assets/Cards/plant-cooldown.png");
    mCardTexturesCooldown[AttackType::Neutral] =
        mGame->GetRenderer()->GetTexture("../Assets/Cards/neutral-cooldown.png");

    SDL_Log("Texturas de cartas carregadas");
}

Texture* CombatScene::GetCardTexture(AttackType type, bool isAvailable)
{
    if (isAvailable) {
        auto it = mCardTexturesActive.find(type);
        if (it != mCardTexturesActive.end()) {
            return it->second;
        }
    } else {
        auto it = mCardTexturesCooldown.find(type);
        if (it != mCardTexturesCooldown.end()) {
            return it->second;
        }
    }
    return nullptr;
}

void CombatScene::AtualizarExibicaoCartas(float deltaTime)
{
    mCardDisplayTimer += deltaTime;

    if (mCardDisplayTimer >= CombatConstants::Timers::CARD_DISPLAY_DURATION)
    {
        mShowingCards = false;
        mCardDisplayTimer = 0.0f;

        LaunchProjectile();
        mShowingProjectile = true;
        mProjectileTimer = 0.0f;
    }
}

void CombatScene::AtualizarProjetil(float deltaTime)
{
    mProjectileTimer += deltaTime;

    if (mProjectile && mProjectile->IsComplete())
    {
        mShowingProjectile = false;

        TriggerDefenderAnimation();

        if (mDisplayPlayerCard)
        {
            mCombatManager->PlayerSelectCard(mDisplayPlayerCard, mDisplayEnemyCard);
        }

        mProjectile->SetState(ActorState::Destroy);
        mProjectile = nullptr;
    }
}

void CombatScene::AtualizarEstadoCombate()
{
    static CombatState previousState = CombatState::WAITING_FOR_PLAYER;
    CombatState currentState = mCombatManager->GetCurrentState();

    mCombatManager->Update();

    if (previousState != CombatState::WAITING_FOR_PLAYER &&
        currentState == CombatState::WAITING_FOR_PLAYER)
    {
        mCardsShown = false;
    }

    if (currentState == CombatState::WAITING_FOR_PLAYER && !mCardsShown)
    {
        mCardsShown = true;
    }

    previousState = currentState;
}

void CombatScene::Update(float deltaTime)
{
    mStateTime += deltaTime;
    UpdateFade(deltaTime);

    if (!mCombatManager) return;

    if (mShowingCards)
    {
        AtualizarExibicaoCartas(deltaTime);
        return;
    }

    if (mShowingProjectile)
    {
        AtualizarProjetil(deltaTime);
        return;
    }

    AtualizarEstadoCombate();

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
    // Renderizar barras de HP
    if (mPlayer && mEnemy && mCombatRenderer) {
        Vector2 playerHPPos = Vector2(CombatConstants::Positions::FROG_X, CombatConstants::Offsets::HP_BAR_Y_OFFSET);
        mCombatRenderer->RenderizarBarraHP(playerHPPos, mPlayer->GetHealth(), 20, false);

        Vector2 enemyHPPos = Vector2(CombatConstants::Positions::BEAR_X, CombatConstants::Offsets::HP_BAR_Y_OFFSET);
        mCombatRenderer->RenderizarBarraHP(enemyHPPos, mEnemy->GetHealth(), 15, true);
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
    if (!mCombatRenderer) return;

    // Posições das cartas
    float playerCardX = CombatConstants::Positions::SCREEN_CENTER_X -
                        CombatConstants::Cards::DISPLAY_SPACING -
                        CombatConstants::Cards::LARGE_WIDTH / 2.0f;
    float enemyCardX = CombatConstants::Positions::SCREEN_CENTER_X +
                       CombatConstants::Cards::DISPLAY_SPACING +
                       CombatConstants::Cards::LARGE_WIDTH / 2.0f;
    float centerY = CombatConstants::Positions::SCREEN_CENTER_Y;

    // Renderizar carta do Player (esquerda)
    if (mDisplayPlayerCard) {
        Vector2 playerCardPos(playerCardX, centerY);
        Texture* cardTexture = GetCardTexture(mDisplayPlayerCard->GetType(), true);
        float brilho = mPlayerWonLastTurn ? 1.3f : 1.0f;

        mCombatRenderer->RenderizarTexturaSimples(
            playerCardPos,
            Vector2(CombatConstants::Cards::LARGE_WIDTH, CombatConstants::Cards::LARGE_HEIGHT),
            cardTexture,
            brilho
        );

        mCombatRenderer->RenderizarTextoPoder(
            Vector2(playerCardX, centerY + CombatConstants::Cards::LARGE_HEIGHT / 2.0f - CombatConstants::Offsets::POWER_TEXT_FROM_BOTTOM_LARGE),
            mDisplayPlayerCard->GetDamage(),
            Vector3(1.0f, 1.0f, 1.0f),
            CombatConstants::FontSizes::POWER_LARGE
        );
    }

    // Renderizar carta do Enemy (direita)
    if (mDisplayEnemyCard) {
        Vector2 enemyCardPos(enemyCardX, centerY);
        Texture* cardTexture = GetCardTexture(mDisplayEnemyCard->GetType(), true);
        float brilho = !mPlayerWonLastTurn ? 1.3f : 1.0f;

        mCombatRenderer->RenderizarTexturaSimples(
            enemyCardPos,
            Vector2(CombatConstants::Cards::LARGE_WIDTH, CombatConstants::Cards::LARGE_HEIGHT),
            cardTexture,
            brilho
        );

        mCombatRenderer->RenderizarTextoPoder(
            Vector2(enemyCardX, centerY + CombatConstants::Cards::LARGE_HEIGHT / 2.0f - CombatConstants::Offsets::POWER_TEXT_FROM_BOTTOM_LARGE),
            mDisplayEnemyCard->GetDamage(),
            Vector3(1.0f, 1.0f, 1.0f),
            CombatConstants::FontSizes::POWER_LARGE
        );
    }

    // Renderizar moldura de vencedor
    if (mWinnerFrameTexture) {
        float winnerX = mPlayerWonLastTurn ? playerCardX : enemyCardX;
        mCombatRenderer->RenderizarMolduraVencedor(
            Vector2(winnerX, centerY),
            Vector2(CombatConstants::Cards::LARGE_WIDTH, CombatConstants::Cards::LARGE_HEIGHT),
            mWinnerFrameTexture
        );
    }
}

void CombatScene::RenderCards()
{
    if (!mCombatRenderer || !mPlayer) return;

    // Calcular posição inicial X para centralizar as 4 cartas
    float totalWidth = (CombatConstants::Cards::SMALL_WIDTH * 4) + (CombatConstants::Cards::SPACING * 3);
    float startX = (CombatConstants::Dimensions::SCREEN_WIDTH - totalWidth) / 2.0f + CombatConstants::Cards::SMALL_WIDTH / 2.0f;

    for (int i = 0; i < 4; i++) {
        Card* card = mPlayer->GetDeck()[i];

        // Calcular posição da carta
        float cardX = startX + (i * (CombatConstants::Cards::SMALL_WIDTH + CombatConstants::Cards::SPACING));
        float cardY = CombatConstants::Positions::CARDS_BASE_Y;

        // Carta selecionada fica mais acima
        if (i == mSelectedCardIndex) {
            cardY += CombatConstants::Cards::SELECTED_OFFSET;
        }

        Vector2 cardPos(cardX, cardY);

        // Obter textura da carta baseada em tipo e disponibilidade
        Texture* cardTexture = GetCardTexture(card->GetType(), card->IsAvailable());

        // Renderizar carta
        mCombatRenderer->RenderizarTexturaSimples(
            cardPos,
            Vector2(CombatConstants::Cards::SMALL_WIDTH, CombatConstants::Cards::SMALL_HEIGHT),
            cardTexture
        );

        // Renderizar número de poder ou ícone
        if (card->IsAvailable()) {
            bool isSelecionada = (i == mSelectedCardIndex);
            Vector3 corTexto = isSelecionada ?
                Vector3(1.0f, 0.84f, 0.0f) :  // Amarelo dourado
                Vector3(1.0f, 1.0f, 1.0f);    // Branco

            mCombatRenderer->RenderizarTextoPoder(
                Vector2(cardX, cardY + CombatConstants::Cards::SMALL_HEIGHT / 2.0f - CombatConstants::Offsets::POWER_TEXT_FROM_BOTTOM),
                card->GetDamage(),
                corTexto,
                CombatConstants::FontSizes::POWER_SMALL
            );
        } else {
            mCombatRenderer->RenderizarIconeCooldown(
                cardPos,
                CombatConstants::Cards::SMALL_HEIGHT,
                mTimeIconTexture
            );
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

    // Bloquear inputs durante a animação do projétil
    if (mShowingProjectile)
        return;

    // Bloquear inputs enquanto os personagens estão em animação
    if ((mFrogActor && mFrogActor->IsInAnimation()) ||
        (mBearActor && mBearActor->IsInAnimation()))
        return;

    // Só processar input se estiver esperando o jogador
    if (mCombatManager->GetCurrentState() != CombatState::WAITING_FOR_PLAYER)
        return;

    // Navegação entre cartas (Left/Right ou A/D)
    if ((keyState[SDL_SCANCODE_RIGHT] || keyState[SDL_SCANCODE_D]) && !mKeyWasPressed)
    {
        mSelectedCardIndex = (mSelectedCardIndex + 1) % 4;
        mKeyWasPressed = true;
    }
    else if ((keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A]) && !mKeyWasPressed)
    {
        mSelectedCardIndex = (mSelectedCardIndex - 1 + 4) % 4;
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

void CombatScene::LaunchProjectile()
{
    AttackType type = mPlayerWonLastTurn ?
        mDisplayPlayerCard->GetType() :
        mDisplayEnemyCard->GetType();

    Vector2 startPos = mPlayerWonLastTurn ?
        Vector2(160.0f, 224.0f) :  // Frog position
        Vector2(480.0f, 224.0f);   // Bear position

    Vector2 endPos = mPlayerWonLastTurn ?
        Vector2(480.0f, 224.0f) :  // Bear position
        Vector2(160.0f, 224.0f);   // Frog position

    // Rotacionar 180 graus quando o ataque vai da direita para esquerda (enemy atacando)
    bool rotate180 = !mPlayerWonLastTurn;

    // Tocar animação de ataque do atacante quando dispara o projétil
    if (mPlayerWonLastTurn)
    {
        if (mFrogActor) mFrogActor->PlayAttack();
        SDL_Log("Player attacking!");
    }
    else
    {
        if (mBearActor) mBearActor->PlayAttack();
        SDL_Log("Enemy attacking!");
    }

    mProjectile = new MagicProjectileActor(mGame, type, startPos, endPos, 1.0f, rotate180);

    SDL_Log("Launching magic projectile from (%.0f, %.0f) to (%.0f, %.0f) %s",
            startPos.x, startPos.y, endPos.x, endPos.y,
            rotate180 ? "(flipped horizontal)" : "");
}

void CombatScene::TriggerDefenderAnimation()
{
    if (mPlayerWonLastTurn)
    {
        // Player venceu: Enemy é atingido
        int enemyHP = mEnemy->GetHealth();
        int damage = mDisplayPlayerCard ? mDisplayPlayerCard->GetDamage() : 0;

        if (enemyHP - damage <= 0)
        {
            // Enemy morre
            if (mBearActor) mBearActor->PlayDeath();
            SDL_Log("Enemy will die from this hit");
        }
        else
        {
            // Enemy toma dano
            if (mBearActor) mBearActor->PlayHurt();
            SDL_Log("Enemy takes damage");
        }
    }
    else
    {
        // Enemy venceu: Player é atingido
        int playerHP = mPlayer->GetHealth();
        int damage = mDisplayEnemyCard ? mDisplayEnemyCard->GetDamage() : 0;

        if (playerHP - damage <= 0)
        {
            // Player morre
            if (mFrogActor) mFrogActor->PlayDeath();
            SDL_Log("Player will die from this hit");
        }
        else
        {
            // Player toma dano
            if (mFrogActor) mFrogActor->PlayHurt();
            SDL_Log("Player takes damage");
        }
    }
}

void CombatScene::Exit()
{
    SDL_Log("Exiting CombatScene");

    // Limpar sistema de renderização
    if (mCombatRenderer)
    {
        delete mCombatRenderer;
        mCombatRenderer = nullptr;
    }

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

    // Limpar projétil se existir
    if (mProjectile)
    {
        mProjectile->SetState(ActorState::Destroy);
        mProjectile = nullptr;
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

    mGame->GetRenderer()->SetClearColor(0.3f, 0.2f, 0.4f, 1.0f);
}

void GameOverScene::Update(float deltaTime)
{
    mStateTime += deltaTime;
}

void GameOverScene::ProcessInput(const Uint8* keyState)
{

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

    mGame->GetRenderer()->SetClearColor(0.8f, 0.7f, 0.2f, 1.0f);
}

void VictoryScene::Update(float deltaTime)
{
    mStateTime += deltaTime;
}

void VictoryScene::ProcessInput(const Uint8* keyState)
{

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
}