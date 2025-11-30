#include "Combatant.h"
#include <vector>
#include <algorithm>

Combatant::Combatant(Game* game, std::string name, int health, int maxHealth, std::vector<Card*> deck)
    : Actor(game)
    , mName(name)
    , mHealth(health)
    , mMaxHealth(maxHealth)
    , mDeck(deck)
    , mIsDead(false)
{
}

void Combatant::TakeDamage(Card* card) {
    mHealth -= card->GetDamage();
    if (mHealth <= 0) {
        mHealth = 0;
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

void Combatant::RemoveCard(Card* card) {
    auto iter = std::find(mDeck.begin(), mDeck.end(), card);
    if (iter != mDeck.end()){
        std::iter_swap(iter, mDeck.end() - 1);
        mDeck.pop_back();

        card->SetOwner(nullptr);
    }
}

void Combatant::IncreaseMaxHealth(int amount) {
    mMaxHealth += amount;
    // Opcional: Quando aumentamos a vida máxima, geralmente aumentamos a vida atual
    // junto para "preencher" o novo espaço vazio.
    mHealth += amount;
}

void Combatant::GiveCard(Card* card) {
    mDeck.push_back(card);
    card->SetOwner(this);
}

void Combatant::Death() {
    mIsDead = true;
    // TODO: Implementar efeitos visuais de morte (animação, som) e transição para tela de game over/vitória
}

