#include "CombatScene.h"
#include "GameOverScene.h"
#include "VictoryScene.h"
#include "RewardScene.h"
#include "../Game.h"
#include "../Combat/CombatManager.h"
#include "../Combat/CombatRenderer.h"
#include "../Combat/CombatConstants.h"
#include "../Actors/Player.h"
#include "../Actors/Enemy.h"
#include "../Actors/FrogActor.h"
#include "../Actors/EnemyFactory.h"
#include "../Actors/MagicProjectileActor.h"
#include "../Map/MapNode.h"
#include "../Random.h"
#include "../Renderer/Texture.h"
#include "../Renderer/Renderer.h"
#include <SDL.h>
#include <SDL_log.h>

CombatScene::CombatScene(Game* game)
    : GameScene(game)
    , mCombatManager(nullptr)
    , mPlayer(nullptr)
    , mEnemy(nullptr)
    , mCombatRenderer(nullptr)
    , mFrogActor(nullptr)
    , mEnemyActor(nullptr)
    , mBackgroundTexture(nullptr)
    , mSelectedCardIndex(0)
    , mKeyWasPressed(false)
    , mCardsShown(false)
    , mShowingCards(false)
    , mCardDisplayTimer(0.0f)
    , mDisplayPlayerCard(nullptr)
    , mDisplayEnemyCard(nullptr)
    , mPlayerWonLastTurn(false)
    , mWasTie(false)
    , mProjectile(nullptr)
    , mShowingProjectile(false)
    , mProjectileTimer(0.0f)
    , mCombatEndHandled(false)
{
}

CombatScene::~CombatScene()
{
}

void CombatScene::Enter()
{
    mStateTime = 0.0f;

    SDL_SetWindowTitle(mGame->GetWindow(), "Project Frog");

    MapNode* currentNode = mGame->GetCurrentMapNode();
    bool isBossFight = (currentNode && currentNode->GetType() == MapNodeType::BOSS);

    if (mGame->GetAudio())
    {
        if (isBossFight) {
            mCombatMusic = mGame->GetAudio()->PlaySound("BossMusic.mp3", true);
        } else {
            mCombatMusic = mGame->GetAudio()->PlaySound("CombatMusic01.ogg", true);
        }
    }

    if (isBossFight) {
        mBackgroundTexture = mGame->GetRenderer()->GetTexture("../Assets/Background/Combat/dungeon.png");
    } else {
        const char* backgroundFiles[] = {
            "../Assets/Background/Combat/pedras.png",
            "../Assets/Background/Combat/floresta.png",
            "../Assets/Background/Combat/pantano.jpeg",
        };
        int randomIndex = Random::GetIntRange(0, 2);
        mBackgroundTexture = mGame->GetRenderer()->GetTexture(backgroundFiles[randomIndex]);
    }

    mGame->GetRenderer()->SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    mCombatRenderer = new CombatRenderer(mGame);

    CreateTestCombatants();

    CreateVisualActors();

    LoadCardTextures();

    mTimeIconTexture = mGame->GetRenderer()->GetTexture("../Assets/Icons/White/icon_time.png");

    mWinnerFrameTexture = mGame->GetRenderer()->GetTexture("../Assets/Cards/winner-frame.png");
}

void CombatScene::CreateTestCombatants()
{
    mPlayer = mGame->GetPlayer();

    if (!mPlayer) {
        SDL_Log("CRITICAL ERROR: Player instance not found in Game!");
        return;
    }

    for (Card* card : mPlayer->GetDeck()) {
        card->SetOwner(mPlayer);
    }

    for (Card* card : mPlayer->GetDeck()) {
        card->ResetCooldown();
    }

    mSelectedCardIndex = 0;
}

void CombatScene::CreateVisualActors()
{
    mFrogActor = new FrogActor(mGame);
    mFrogActor->SetPosition(Vector2(150.0f, 224.0f));
    mFrogActor->SetScale(Vector2(1.0f, 1.0f));

    MapNode* currentNode = mGame->GetCurrentMapNode();
    bool isBossFight = (currentNode && currentNode->GetType() == MapNodeType::BOSS);

    if (isBossFight) {
        mEnemyActor = EnemyFactory::CreateEnemy(mGame, EnemyType::GOLEM);
    } else {
        mEnemyActor = EnemyFactory::CreateRandomEnemy(mGame);
    }

    mEnemyActor->SetPosition(Vector2(490.0f, 224.0f));

    AnimatedCharacterActor::EnemyStats stats = mEnemyActor->GetEnemyStats();

    std::vector<Card*> enemyDeck = mEnemyActor->CreateEnemyDeck();

    mEnemy = new Enemy(mGame, "Enemy", stats.health, stats.maxHealth, enemyDeck, stats.difficulty);

    for (Card* card : mEnemy->GetDeck()) {
        card->SetOwner(mEnemy);
    }

    mCombatManager = new CombatManager(mPlayer, mEnemy, 10);
    mCombatManager->StartCombat();
}

void CombatScene::LoadCardTextures()
{
    mCardTexturesActive[AttackType::Fire] =
        mGame->GetRenderer()->GetTexture("../Assets/Cards/fire-active.png");
    mCardTexturesActive[AttackType::Water] =
        mGame->GetRenderer()->GetTexture("../Assets/Cards/water-active.png");
    mCardTexturesActive[AttackType::Plant] =
        mGame->GetRenderer()->GetTexture("../Assets/Cards/plant-active.png");
    mCardTexturesActive[AttackType::Neutral] =
        mGame->GetRenderer()->GetTexture("../Assets/Cards/neutral-active.png");

    mCardTexturesCooldown[AttackType::Fire] =
        mGame->GetRenderer()->GetTexture("../Assets/Cards/fire-cooldown.png");
    mCardTexturesCooldown[AttackType::Water] =
        mGame->GetRenderer()->GetTexture("../Assets/Cards/water-cooldown.png");
    mCardTexturesCooldown[AttackType::Plant] =
        mGame->GetRenderer()->GetTexture("../Assets/Cards/plant-cooldown.png");
    mCardTexturesCooldown[AttackType::Neutral] =
        mGame->GetRenderer()->GetTexture("../Assets/Cards/neutral-cooldown.png");
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

        if (!mWasTie)
        {
            LaunchProjectile();
            mShowingProjectile = true;
            mProjectileTimer = 0.0f;
        }
        else
        {
            if (mDisplayPlayerCard)
            {
                mCombatManager->PlayerSelectCard(mDisplayPlayerCard, mDisplayEnemyCard);
            }
        }
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
    if (mBackgroundTexture) {
        mGame->GetRenderer()->DrawTexture(
            Vector2(320.0f, 224.0f),
            Vector2(640.0f, 448.0f),
            0.0f,
            Vector3(1.0f, 1.0f, 1.0f),
            mBackgroundTexture,
            Vector4::UnitRect,
            Vector2::Zero
        );
    }
}

void CombatScene::Render()
{
    RenderCombatUI();
}

void CombatScene::RenderCombatUI()
{
    if (mPlayer && mEnemy && mCombatRenderer && mFrogActor && mEnemyActor) {
        float playerBarOffset = (mFrogActor->GetSpriteHeight() / 2.0f) + 2.0f;
        Vector2 playerHPPos = Vector2(mFrogActor->GetPosition().x, mFrogActor->GetPosition().y - playerBarOffset);
        mCombatRenderer->RenderizarBarraHP(playerHPPos, mPlayer->GetHealth(), mPlayer->GetMaxHealth(), false);

        float enemyBarOffset = (mEnemyActor->GetSpriteHeight() / 2.0f) + 2.0f;
        Vector2 enemyHPPos = Vector2(mEnemyActor->GetPosition().x, mEnemyActor->GetPosition().y - enemyBarOffset);
        mCombatRenderer->RenderizarBarraHP(enemyHPPos, mEnemy->GetHealth(), mEnemy->GetMaxHealth(), true);
    }

    if (mShowingCards) {
        RenderCardDisplay();
    }
    else if (mPlayer && mCombatManager && mCombatManager->GetCurrentState() == CombatState::WAITING_FOR_PLAYER) {
        RenderCards();
    }
}

Vector3 CombatScene::GetCardColor(AttackType type)
{
    switch (type) {
        case AttackType::Fire:
            return Vector3(1.0f, 0.2f, 0.2f);
        case AttackType::Water:
            return Vector3(0.2f, 0.4f, 1.0f);
        case AttackType::Plant:
            return Vector3(0.2f, 1.0f, 0.2f);
        case AttackType::Neutral:
            return Vector3(0.1f, 0.1f, 0.1f);
        default:
            return Vector3(0.5f, 0.5f, 0.5f);
    }
}

void CombatScene::RenderCardDisplay()
{
    if (!mCombatRenderer) return;

    float playerCardX = CombatConstants::Positions::SCREEN_CENTER_X -
                        CombatConstants::Cards::DISPLAY_SPACING -
                        CombatConstants::Cards::LARGE_WIDTH / 2.0f;
    float enemyCardX = CombatConstants::Positions::SCREEN_CENTER_X +
                       CombatConstants::Cards::DISPLAY_SPACING +
                       CombatConstants::Cards::LARGE_WIDTH / 2.0f;
    float centerY = CombatConstants::Positions::SCREEN_CENTER_Y;

    Vector2 cardSizeLarge(CombatConstants::Cards::LARGE_WIDTH, CombatConstants::Cards::LARGE_HEIGHT);

    if (mDisplayPlayerCard) {
        float brilho = mPlayerWonLastTurn ? 1.3f : 1.0f;

        mCombatRenderer->RenderizarCarta(
            Vector2(playerCardX, centerY),
            cardSizeLarge,
            mDisplayPlayerCard,
            false,
            brilho,
            mCardTexturesActive,
            mCardTexturesCooldown,
            mTimeIconTexture,
            false
        );
    }

    if (mDisplayEnemyCard) {
        float brilho = !mPlayerWonLastTurn ? 1.3f : 1.0f;

        mCombatRenderer->RenderizarCarta(
            Vector2(enemyCardX, centerY),
            cardSizeLarge,
            mDisplayEnemyCard,
            false,
            brilho,
            mCardTexturesActive,
            mCardTexturesCooldown,
            mTimeIconTexture,
            false
        );
    }

    if (mWinnerFrameTexture && !mWasTie) {
        float winnerX = mPlayerWonLastTurn ? playerCardX : enemyCardX;
        mCombatRenderer->RenderizarMolduraVencedor(
            Vector2(winnerX, centerY),
            cardSizeLarge,
            mWinnerFrameTexture
        );
    }
}

void CombatScene::RenderCards()
{
    if (!mCombatRenderer || !mPlayer) return;

    float totalWidth = (CombatConstants::Cards::SMALL_WIDTH * 4) + (CombatConstants::Cards::SPACING * 3);
    float startX = (CombatConstants::Dimensions::SCREEN_WIDTH - totalWidth) / 2.0f + CombatConstants::Cards::SMALL_WIDTH / 2.0f;

    for (int i = 0; i < 4; i++) {
        Card* card = mPlayer->GetDeck()[i];

        float cardX = startX + (i * (CombatConstants::Cards::SMALL_WIDTH + CombatConstants::Cards::SPACING));
        float cardY = CombatConstants::Positions::CARDS_BASE_Y;

        if (i == mSelectedCardIndex) {
            cardY += CombatConstants::Cards::SELECTED_OFFSET;
        }

        Vector2 cardPos(cardX, cardY);
        bool isSelecionada = (i == mSelectedCardIndex);

        mCombatRenderer->RenderizarCarta(
            cardPos,
            Vector2(CombatConstants::Cards::SMALL_WIDTH, CombatConstants::Cards::SMALL_HEIGHT),
            card,
            isSelecionada,
            1.0f,
            mCardTexturesActive,
            mCardTexturesCooldown,
            mTimeIconTexture,
            true
        );
    }
}

void CombatScene::ProcessInput(const Uint8* keyState)
{
    if (!mCombatManager)
        return;

    if (mFadeAlpha > 0.0f)
        return;

    if (mShowingCards)
        return;

    if (mShowingProjectile)
        return;

    if ((mFrogActor && mFrogActor->IsInAnimation()) ||
        (mEnemyActor && mEnemyActor->IsInAnimation()))
        return;

    if (mCombatManager->GetCurrentState() != CombatState::WAITING_FOR_PLAYER)
        return;

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
    else if ((keyState[SDL_SCANCODE_RETURN] || keyState[SDL_SCANCODE_SPACE]) && !mKeyWasPressed)
    {
        Card* selectedCard = mPlayer->GetDeck()[mSelectedCardIndex];

        if (selectedCard->IsAvailable())
        {
            mDisplayPlayerCard = selectedCard;
            mDisplayEnemyCard = mEnemy->SelectCard();
            mShowingCards = true;
            mCardDisplayTimer = 0.0f;

            if (mDisplayEnemyCard) {
                bool playerHasAdvantage = mDisplayPlayerCard->HasTypeAdvantageOver(mDisplayEnemyCard);
                bool enemyHasAdvantage = mDisplayEnemyCard->HasTypeAdvantageOver(mDisplayPlayerCard);

                if (playerHasAdvantage) {
                    mPlayerWonLastTurn = true;
                    mWasTie = false;
                } else if (enemyHasAdvantage) {
                    mPlayerWonLastTurn = false;
                    mWasTie = false;
                } else {
                    int playerDamage = mDisplayPlayerCard->GetDamage();
                    int enemyDamage = mDisplayEnemyCard->GetDamage();

                    if (playerDamage == enemyDamage) {
                        mWasTie = true;
                        mPlayerWonLastTurn = false;
                    } else {
                        mWasTie = false;
                        mPlayerWonLastTurn = (playerDamage > enemyDamage);
                    }
                }
            } else {
                mPlayerWonLastTurn = true;
                mWasTie = false;
            }

            mKeyWasPressed = true;
        }
        else
        {
            SDL_Log("❌ Carta em cooldown! (%d turnos restantes)",
                    selectedCard->GetCurrentCooldown());
            mKeyWasPressed = true;
        }
    }
    else if (!keyState[SDL_SCANCODE_RIGHT] && !keyState[SDL_SCANCODE_LEFT] &&
             !keyState[SDL_SCANCODE_D] && !keyState[SDL_SCANCODE_A] &&
             !keyState[SDL_SCANCODE_RETURN] && !keyState[SDL_SCANCODE_SPACE])
    {
        mKeyWasPressed = false;
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
    if (mCombatEndHandled)
        return;

    mCombatEndHandled = true;

    if (mCombatManager->IsPlayerVictorious())
    {
        SDL_Delay(1000);

        MapNode* currentNode = mGame->GetCurrentMapNode();
        if (currentNode && currentNode->GetType() == MapNodeType::BOSS) {
            mGame->SetScene(new VictoryScene(mGame));
        } else {
            mGame->SetScene(new RewardScene(mGame, RewardMode::COMBAT_VICTORY));
        }
    }
    else
    {
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
        Vector2(160.0f, 224.0f) :
        Vector2(480.0f, 224.0f);

    Vector2 endPos = mPlayerWonLastTurn ?
        Vector2(480.0f, 224.0f) :
        Vector2(160.0f, 224.0f);

    bool rotate180 = !mPlayerWonLastTurn;

    if (mPlayerWonLastTurn)
    {
        if (mFrogActor) mFrogActor->PlayAttack();
    }
    else
    {
        if (mEnemyActor) mEnemyActor->PlayAttack();
    }

    mProjectile = new MagicProjectileActor(mGame, type, startPos, endPos, 1.0f, rotate180);
}

void CombatScene::TriggerDefenderAnimation()
{
    if (mPlayerWonLastTurn)
    {
        int enemyHP = mEnemy->GetHealth();
        int damage = mDisplayPlayerCard ? mDisplayPlayerCard->GetDamage() : 0;

        if (enemyHP - damage <= 0)
        {
            if (mEnemyActor) mEnemyActor->PlayDeath();
        }
        else
        {
            if (mEnemyActor) mEnemyActor->PlayHurt();
        }
    }
    else
    {
        int playerHP = mPlayer->GetHealth();
        int damage = mDisplayEnemyCard ? mDisplayEnemyCard->GetDamage() : 0;

        if (playerHP - damage <= 0)
        {
            if (mFrogActor) mFrogActor->PlayDeath();
        }
        else
        {
            if (mFrogActor) mFrogActor->PlayHurt();
        }
    }
}

void CombatScene::Exit()
{
    if (mGame->GetAudio())
    {
        mGame->GetAudio()->StopSound(mCombatMusic);
    }

    if (mCombatRenderer)
    {
        delete mCombatRenderer;
        mCombatRenderer = nullptr;
    }

    if (mFrogActor)
    {
        mFrogActor->SetState(ActorState::Destroy);
        mFrogActor = nullptr;
    }

    if (mEnemyActor)
    {
        mEnemyActor->SetState(ActorState::Destroy);
        mEnemyActor = nullptr;
    }

    if (mProjectile)
    {
        mProjectile->SetState(ActorState::Destroy);
        mProjectile = nullptr;
    }

    mBackgroundTexture = nullptr;

    if (mCombatManager)
    {
        delete mCombatManager;
        mCombatManager = nullptr;
    }

    mPlayer = nullptr;

    if (mEnemy)
    {
        for (Card* card : mEnemy->GetDeck())
        {
            delete card;
        }
        delete mEnemy;
        mEnemy = nullptr;
    }
}

