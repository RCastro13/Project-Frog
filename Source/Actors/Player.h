#pragma once
#include "Combatant.h"
#include "../Combat/Item.h"

class Player : public Combatant {
    public:
    Player(Game* game, std::string name, int health, int maxHealth, std::vector<Card*> deck = std::vector<Card*>());

    void Death() override;
    void GiveCoins(int amount) { mCoins += amount; }
    int GetCoins() const { return mCoins; }

    private:
    std::vector<Item*> mInventory;
    int mCoins;
};