#include "Combatant.h"
#include <vector>
#include <algorithm>
using namespace std;

Combatant::Combatant(Game* game, std::string name, int health, int maxHealth, std::vector<Card*> deck)
    :Actor(game)
    ,mName(name)
    ,mHealth(health)
    ,mMaxHealth(maxHealth)
    ,mDeck(deck)
    ,mIsDead(false)
{

}

void Combatant::TakeDamage(Card* card) {
    mHealth -= card->GetDamage();
    if (mHealth <= 0) {
        mHealth = 0;
        // TODO : What happends when he dies?
        Death();
    }
}

void Combatant::RestoreHealth() {
    Heal(mMaxHealth);
}

void Combatant::Heal(int heal) {
    mHealth += heal;
    if (mHealth >= mMaxHealth) {
        mHealth = mMaxHealth;
    }
}

void Combatant::QuitCard(Card *card) {
    auto iter = std::find(mDeck.begin(), mDeck.end(), card);
    if (iter != mDeck.end()){
        std::iter_swap(iter, mDeck.end() - 1);
        mDeck.pop_back();

        card->SetOwner(nullptr);
    }
}

void Combatant::GiveCard(Card *card) {
    mDeck.push_back(card);
    card->SetOwner(this);
}

void Combatant::Death() {
    mIsDead = true;
    // TODO : É necessário definir claramente o que acontece quando o personagem morre.
}

