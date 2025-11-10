#include "Enemy.h"

Enemy::Enemy(Game *game, std::string name, int health, int maxHealth, std::vector<Card *> deck)
    : Combatant(game, name, health, maxHealth, deck)
{

}

Card* Enemy::SelectCard() {
    // TODO : Vamos ver o que a IA fará.
    return GetDeck()[0];
}

void Enemy::LearnFromAtack(Card* card) {
    // TODO : Aqui, a IA aprende com a última ação realizada pelo jogador.
}

void Enemy::Death() {
    //TODO : O que acontece quando o Enemy morre?
}
