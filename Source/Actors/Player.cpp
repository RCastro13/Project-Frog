#include "Player.h"

Player::Player(Game *game, std::string name, int health, int maxHealth, std::vector<Card *> deck)
    : Combatant(game, name, health, maxHealth, deck)
    ,mInventory(std::vector<Item*>())
    ,mCoins(100)  // Player começa com 50 moedas
{

}

Player::~Player() {
    if (this->GetDeck().size() > 0) {
        for (Card* card : this->GetDeck()) {
            delete card;
        }
    }
}

bool Player::SpendCoins(int amount)
{
    if (mCoins >= amount)
    {
        mCoins -= amount;
        return true;
    }
    return false;
}

void Player::Death() {
    // TODO: Transição para tela de game over, salvar progresso da run, mostrar estatísticas
}
