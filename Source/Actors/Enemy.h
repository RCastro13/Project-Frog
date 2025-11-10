#pragma once
#include "Combatant.h"
#include "../Combat/Item.h"

class Enemy : public Combatant {
public:
    Enemy(Game* game, std::string name, int health, int maxHealth, std::vector<Card*> deck = std::vector<Card*>());

    Card * SelectCard();

    void LearnFromAtack(Card* card);

    void Death() override;

private:
};