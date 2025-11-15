#include "CombatManager.h"
#include <algorithm>
#include <SDL_log.h>

CombatManager::CombatManager(Player* player, Enemy* enemy, int coinReward)
    : mPlayer(player)
    , mEnemy(enemy)
    , mCurrentState(CombatState::WAITING_FOR_PLAYER)
    , mPlayerVictorious(false)
    , mCurrentTurn(0)
    , mPlayerSelectedCard(nullptr)
    , mEnemySelectedCard(nullptr)
    , mCoinReward(coinReward)
{
}

void CombatManager::StartCombat()
{
    mCurrentState = CombatState::WAITING_FOR_PLAYER;
    mCurrentTurn = 1;
    mPlayerVictorious = false;
}

void CombatManager::Update()
{
    switch (mCurrentState)
    {
        case CombatState::WAITING_FOR_PLAYER:
            // A cena de combate irá gerenciar a seleção da carta
            break;

        case CombatState::ENEMY_TURN:
            EnemySelectCard();
            mCurrentState = CombatState::RESOLVING_COMBAT;
            break;

        case CombatState::RESOLVING_COMBAT:
            ResolveCombat();
            mCurrentState = CombatState::CHECKING_VICTORY;
            break;

        case CombatState::CHECKING_VICTORY:
            CheckVictoryConditions();
            if (mCurrentState != CombatState::COMBAT_ENDED)
            {
                UpdateCooldowns();
                mCurrentState = CombatState::WAITING_FOR_PLAYER;
                mCurrentTurn++;
            }
            break;

        default:
            break;
    }
}

void CombatManager::PlayerSelectCard(Card* selectedCard, Card* enemyCard)
{
    if (mCurrentState == CombatState::WAITING_FOR_PLAYER)
    {
        mPlayerSelectedCard = selectedCard;

        // Se a carta do inimigo já foi fornecida (feedback visual), usá-la diretamente
        if (enemyCard != nullptr)
        {
            mEnemySelectedCard = enemyCard;
            mCurrentState = CombatState::RESOLVING_COMBAT;
        }
        else
        {
            // Caso contrário, passar pelo estado ENEMY_TURN normalmente
            mCurrentState = CombatState::ENEMY_TURN;
        }
    }
}

void CombatManager::EnemySelectCard()
{
    mEnemySelectedCard = mEnemy->SelectCard();

    // Verificar se a seleção foi bem-sucedida
    if (!mEnemySelectedCard)
    {
        SDL_Log("ERROR: Enemy failed to select a card!");
        return;
    }

    // Log da ação do inimigo
    const char* typeName = "";
    switch (mEnemySelectedCard->GetType())
    {
        case AttackType::Fire:    typeName = "🔥 Fire"; break;
        case AttackType::Water:   typeName = "💧 Water"; break;
        case AttackType::Plant:   typeName = "🌿 Plant"; break;
        case AttackType::Neutral: typeName = "⚪ Neutral"; break;
    }

    SDL_Log("Enemy usou: %s (Tipo: %s, Dano: %d)",
            mEnemySelectedCard->GetName().c_str(),
            typeName,
            mEnemySelectedCard->GetDamage());
}

// CompareTypes foi removido, pois agora é tratado por Card::Fight

void CombatManager::ResolveCombat()
{
    if (!mPlayerSelectedCard || !mEnemySelectedCard)
        return;

    // Guardar HP antes do combate para calcular dano
    int playerHPBefore = mPlayer->GetHealth();
    int enemyHPBefore = mEnemy->GetHealth();

    // As cartas se enfrentam e o combate é resolvido
    Combatant* winner = mPlayerSelectedCard->Fight(mEnemySelectedCard);

    // Calcular dano causado
    int playerDamage = playerHPBefore - mPlayer->GetHealth();
    int enemyDamage = enemyHPBefore - mEnemy->GetHealth();

    // Logar resultado do turno
    SDL_Log("");
    if (winner == mPlayer)
    {
        SDL_Log("✅ PLAYER VENCEU O TURNO! (Causou %d de dano)", enemyDamage);
    }
    else if (winner == mEnemy)
    {
        SDL_Log("❌ ENEMY VENCEU O TURNO! (Causou %d de dano)", playerDamage);
    }
    else
    {
        SDL_Log("🤝 EMPATE! Ninguém causou dano.");
    }

    mEnemy->LearnFromAtack(mPlayerSelectedCard);

    // Aplicar tempos de recarga (cooldowns) às cartas usadas
    mPlayerSelectedCard->StartCooldown();
    mEnemySelectedCard->StartCooldown();
}

void CombatManager::CheckVictoryConditions()
{
    if (mEnemy->GetHealth() <= 0)
    {
        mPlayerVictorious = true;
        mCurrentState = CombatState::COMBAT_ENDED;
        HandleVictory();
    }
    else if (mPlayer->GetHealth() <= 0)
    {
        mPlayerVictorious = false;
        mCurrentState = CombatState::COMBAT_ENDED;
    }
}

void CombatManager::HandleVictory()
{
    if (mPlayerVictorious)
    {
        mPlayer->GiveCoins(mCoinReward);
    }
}

void CombatManager::UpdateCooldowns()
{
    // Atualizar tempos de recarga de todas as cartas do jogador
    for (Card* card : mPlayer->GetDeck())
    {
        card->UpdateCooldown();
    }

    // Atualizar tempos de recarga de todas as cartas do inimigo
    for (Card* card : mEnemy->GetDeck())
    {
        card->UpdateCooldown();
    }
}

void CombatManager::EndCombat()
{
    // Limpeza ao finalizar o combate
    mPlayerSelectedCard = nullptr;
    mEnemySelectedCard = nullptr;
}
