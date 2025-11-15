#pragma once
#include "../Actors/Player.h"
#include "../Actors/Enemy.h"
#include "../Actors/Combatant.h"
#include "Card.h"
#include <memory>

enum class CombatState {
    WAITING_FOR_PLAYER,    // Esperando o jogador selecionar uma carta
    ENEMY_TURN,            // IA do inimigo selecionando carta
    RESOLVING_COMBAT,      // Resolvendo o combate (comparando cartas)
    APPLYING_EFFECTS,      // Aplicando dano, efeitos, etc.
    CHECKING_VICTORY,      // Verificando se alguém venceu
    COMBAT_ENDED           // Combate encerrado (vitória ou derrota)
};

class CombatManager {
public:
    // Construtor
    CombatManager(Player* player, Enemy* enemy, int coinReward);

    // Métodos principais
    void StartCombat();
    void Update(); // Chamado a cada frame para atualizar o estado do combate
    void EndCombat();

    // Getters
    CombatState GetCurrentState() const { return mCurrentState; }
    bool IsCombatEnded() const { return mCurrentState == CombatState::COMBAT_ENDED; }
    bool IsPlayerVictorious() const { return mPlayerVictorious; }
    Player* GetPlayer() const { return mPlayer; }
    Enemy* GetEnemy() const { return mEnemy; }
    int GetReward() const { return mCoinReward; }
    int GetCurrentTurn() const { return mCurrentTurn; }

    // Métodos de controle de turno
    void PlayerSelectCard(Card* selectedCard, Card* enemyCard = nullptr);
    void EnemySelectCard(); // A IA selecionará a carta

private:
    // Métodos internos
    void ResolveCombat(); // Compara as cartas e aplica o dano
    void UpdateCooldowns(); // Reduz os tempos de recarga ativos em 1
    void CheckVictoryConditions();
    void HandleVictory(); // Gerencia as recompensas e efeitos da vitória

    // Estado do combate
    CombatState mCurrentState;
    bool mPlayerVictorious;
    int mCurrentTurn;

    // Combatentes
    Player* mPlayer;
    Enemy* mEnemy;

    // Cartas selecionadas no turno atual
    Card* mPlayerSelectedCard;
    Card* mEnemySelectedCard;

    // Recompensas
    int mCoinReward;
};
