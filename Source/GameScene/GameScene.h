//
// Created by Mi PC on 05/11/2025.
//

#pragma once
#include <SDL_stdinc.h>
#include <vector>
#include <map>
#include <string>
#include "../Math.h"

// Forward declarations
class Game;
class CombatManager;
class Player;
class Enemy;
class Card;
class MapNode;
enum class AttackType;
enum class MapNodeType;

// Enum para identificar tipos de cena
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

    // Chamado durante a renderização (ANTES dos actors - para backgrounds)
    virtual void RenderBackground() {}

    // Chamado durante a renderização (DEPOIS dos actors - para UI)
    virtual void Render() {}

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
    float mFadeAlpha;  // Alpha do fade (1.0 = preto total, 0.0 = transparente)
    float mFadeTimer;  // Timer para controlar o fade
    bool mFadingIn;    // true = fade in, false = fade out

    void UpdateFade(float deltaTime);
    void RenderFade();
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
    void RenderBackground() override;
    void Render() override;
    void ProcessInput(const Uint8* keyState) override;
    void Exit() override;

    SceneType GetType() const override { return SceneType::MAIN_MENU; }
    const char* GetName() const override { return "MainMenu"; }

private:
    void UpdateMenuTextures();  // Atualizar texturas com cores apropriadas

    class Texture* mBackgroundTexture;

    // Sistema de menu com opções
    int mSelectedOption;      // 0 = Jogar, 1 = Sair, 2 = Debugar
    bool mKeyWasPressed;      // Controle de input

    // Texturas das opções do menu
    class Texture* mOptionStartTexture;
    class Texture* mOptionDebugTexture;
    class Texture* mOptionExitTexture;
};

class MapScene : public GameScene
{
public:
    MapScene(Game* game);
    ~MapScene();

    void Enter() override;
    void Update(float deltaTime) override;
    void RenderBackground() override;
    void ProcessInput(const Uint8* keyState) override;
    void Exit() override;

    SceneType GetType() const override { return SceneType::MAP; }
    const char* GetName() const override { return "Map"; }

    // Controle do progresso
    void SetCurrentNode(class MapNode* node);
    class MapNode* GetCurrentNode() const { return mCurrentNode; }

private:
    std::vector<class MapNode*> mMapNodes;
    class MapNode* mCurrentNode;
    class MapNode* mSelectedNode;  // Nó que o cursor/seleção está apontando
    int mSelectedIndex;            // Índice do nó selecionado entre os acessíveis

    // Background
    class Texture* mBackgroundTexture;

    // Camera/Scrolling
    Vector2 mCameraPosition;      // Posição da câmera para scrolling
    float mMinCameraX;            // Limite mínimo de scroll
    float mMaxCameraX;            // Limite máximo de scroll

    // Icon management
    std::vector<std::string> mAvailableIcons;  // Lista de caminhos de ícones disponíveis
    std::map<class MapNode*, class Texture*> mNodeIcons;  // Mapeia cada nó para sua textura de ícone

    // Helper methods
    void RenderMap();
    void RenderNode(class MapNode* node);
    void RenderConnections();
    void SelectNextAccessibleNode();
    void SelectPreviousAccessibleNode();
    void ConfirmSelection();
    bool CanSelectNode(class MapNode* node);
    std::vector<class MapNode*> GetAccessibleNodes();
    const char* GetNodeTypeName(MapNodeType type);
    void LoadAvailableIcons();
    void AssignIconsToNodes();
    std::string GetIconPathForNodeType(MapNodeType type);
};

class CombatScene : public GameScene
{
public:
    CombatScene(Game* game);
    ~CombatScene();

    void Enter() override;
    void Update(float deltaTime) override;
    void RenderBackground() override;
    void Render() override;
    void ProcessInput(const Uint8* keyState) override;
    void Exit() override;

    SceneType GetType() const override { return SceneType::COMBAT; }
    const char* GetName() const override { return "Combat"; }

private:
    // Sistema de combate
    CombatManager* mCombatManager;
    Player* mPlayer;
    Enemy* mEnemy;

    // Renderização
    class CombatRenderer* mCombatRenderer;

    // Atores visuais
    class FrogActor* mFrogActor;
    class AnimatedCharacterActor* mEnemyActor;

    // Background
    class Texture* mBackgroundTexture;

    // Texturas das cartas
    std::map<AttackType, class Texture*> mCardTexturesActive;
    std::map<AttackType, class Texture*> mCardTexturesCooldown;
    class Texture* mTimeIconTexture;  // Ícone de cooldown
    class Texture* mWinnerFrameTexture;  // Moldura de vencedor

    // UI - Seleção de cartas
    int mSelectedCardIndex;
    bool mKeyWasPressed;  // Para detectar single press

    // Feedback visual do turno
    bool mShowingCards;           // Se está mostrando as cartas no centro
    float mCardDisplayTimer;      // Timer para mostrar as cartas
    Card* mDisplayPlayerCard;     // Carta do player sendo mostrada
    Card* mDisplayEnemyCard;      // Carta do enemy sendo mostrada
    bool mPlayerWonLastTurn;      // Se o player venceu o último turno
    bool mWasTie;                 // Se o último turno foi empate

    // Magic projectile system
    class MagicProjectileActor* mProjectile;  // Projétil mágico atual
    bool mShowingProjectile;                  // Se está mostrando o projétil
    float mProjectileTimer;                   // Timer do projétil

    // Combat end
    bool mCombatEndHandled;                   // Se o fim do combate já foi processado

    // Helper methods
    void CreateTestCombatants();
    void CreateVisualActors();
    void LoadCardTextures();
    void RenderCombatUI();
    void RenderCards();
    void RenderCardDisplay();
    void HandleCombatEnd();
    const char* GetTypeName(AttackType type);
    Vector3 GetCardColor(AttackType type);
    class Texture* GetCardTexture(AttackType type, bool isAvailable);

    // Projectile system helpers
    void LaunchProjectile();
    void TriggerDefenderAnimation();

    // Update helpers
    void AtualizarExibicaoCartas(float deltaTime);
    void AtualizarProjetil(float deltaTime);
    void AtualizarEstadoCombate();

    // Estado para controlar quando mostrar cartas
    bool mCardsShown;
};

class GameOverScene : public GameScene
{
public:
    GameOverScene(Game* game);
    ~GameOverScene();

    void Enter() override;
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* keyState) override;
    void RenderBackground() override;
    void Render() override;
    void Exit() override;

    SceneType GetType() const override { return SceneType::GAME_OVER; }
    const char* GetName() const override { return "GameOver"; }

private:
    class Texture* mBackgroundTexture;
    class Texture* mMenuTexture;
    float mPulseTimer;
    bool mKeyWasPressed;
};

class VictoryScene : public GameScene
{
public:
    VictoryScene(Game* game);
    ~VictoryScene();

    void Enter() override;
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* keyState) override;
    void RenderBackground() override;
    void Render() override;
    void Exit() override;

    SceneType GetType() const override { return SceneType::VICTORY; }
    const char* GetName() const override { return "Victory"; }

private:
    class Texture* mBackgroundTexture;
    class Texture* mPlayAgainTexture;
    float mPulseTimer;
    bool mKeyWasPressed;
};

class BlackScreenScene : public GameScene
{
public:
    BlackScreenScene(Game* game);
    ~BlackScreenScene();

    void Enter() override;
    void Update(float deltaTime) override;
    void RenderBackground() override;
    void ProcessInput(const Uint8* keyState) override;
    void Exit() override;

    SceneType GetType() const override { return SceneType::BLACK_SCREEN; }
    const char* GetName() const override { return "BlackScreen"; }
};