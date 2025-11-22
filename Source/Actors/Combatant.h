#pragma once
#include <vector>

#include "Actor.h"
#include "../Combat/Card.h"


class Combatant : public Actor
{
    public:
    Combatant(Game* game, std::string name, int health, int maxHealth, std::vector<Card*> deck = std::vector<Card*>());

    void TakeDamage(Card* card);
    void RestoreHealth();
    void Heal(int heal);

    void RemoveCard(Card* card);
    void GiveCard(Card* card);

    int GetHealth() const { return mHealth; }
    int GetMaxHealth() const { return mMaxHealth; }

    virtual void Death();

    std::vector<Card*> GetDeck(){return mDeck;}

private:
    std::string mName;
    int mHealth;
    int mMaxHealth;
    std::vector<Card*> mDeck;
    bool mIsDead;
};
