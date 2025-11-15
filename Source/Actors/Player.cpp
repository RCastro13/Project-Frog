#include "Player.h"

Player::Player(Game *game, std::string name, int health, int maxHealth, std::vector<Card *> deck)
    : Combatant(game, name, health, maxHealth, deck)
    ,mInventory(std::vector<Item*>())
    ,mCoins(0)
{

}

void Player::Death() {
    //TODO : O que acontece quando o Player morre?
}
