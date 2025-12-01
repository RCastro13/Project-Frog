#pragma once
#include "GameScene.h"
#include "../Combat/Card.h"
#include "../Audio/AudioSystem.h"
#include "../Math.h"
#include <map>

class CombatManager;
class Player;
class Enemy;
class CombatRenderer;
class FrogActor;
class AnimatedCharacterActor;
class MagicProjectileActor;
class Texture;

class CombatScene : public GameScene
{
public:
    CombatScene(class Game* game);
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
    CombatManager* mCombatManager;
    Player* mPlayer;
    Enemy* mEnemy;

    SoundHandle mCombatMusic;

    CombatRenderer* mCombatRenderer;

    FrogActor* mFrogActor;
    AnimatedCharacterActor* mEnemyActor;

    Texture* mBackgroundTexture;

    std::map<AttackType, Texture*> mCardTexturesActive;
    std::map<AttackType, Texture*> mCardTexturesCooldown;
    Texture* mTimeIconTexture;
    Texture* mWinnerFrameTexture;

    int mSelectedCardIndex;
    bool mKeyWasPressed;

    bool mShowingCards;
    float mCardDisplayTimer;
    Card* mDisplayPlayerCard;
    Card* mDisplayEnemyCard;
    bool mPlayerWonLastTurn;
    bool mWasTie;

    MagicProjectileActor* mProjectile;
    bool mShowingProjectile;
    float mProjectileTimer;

    bool mCombatEndHandled;

    void CreateTestCombatants();
    void CreateVisualActors();
    void LoadCardTextures();
    void RenderCombatUI();
    void RenderCards();
    void RenderCardDisplay();
    void HandleCombatEnd();
    const char* GetTypeName(AttackType type);
    Vector3 GetCardColor(AttackType type);
    Texture* GetCardTexture(AttackType type, bool isAvailable);

    void LaunchProjectile();
    void TriggerDefenderAnimation();

    void AtualizarExibicaoCartas(float deltaTime);
    void AtualizarProjetil(float deltaTime);
    void AtualizarEstadoCombate();

    bool mCardsShown;
};

