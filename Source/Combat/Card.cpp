#include "Card.h"
#include "../Actors/Combatant.h"

Card::Card(std::string name, AttackType type, int value, int coolDown, Combatant* owner)
    : mName(name)
    , mType(type)
    , mValue(value)
    , mCoolDown(coolDown)
    , mCurrentCooldown(0)
    , mOwner(owner)
{
}

bool Card::HasTypeAdvantageOver(const Card* otherCard) const
{
    if (mType == AttackType::Neutral || otherCard->GetType() == AttackType::Neutral)
        return false;

    if (mType == AttackType::Fire && otherCard->GetType() == AttackType::Plant)
        return true;
    if (mType == AttackType::Plant && otherCard->GetType() == AttackType::Water)
        return true;
    if (mType == AttackType::Water && otherCard->GetType() == AttackType::Fire)
        return true;

    return false;
}

void Card::ResetCooldown()
{
    mCurrentCooldown = 0;
}

Combatant* Card::Fight(Card* otherCard)
{
    if (!mOwner || !otherCard->GetOwner())
        return nullptr;

    // Primeiro verificamos vantagens de tipo
    if (HasTypeAdvantageOver(otherCard))
    {
        // Esta carta vence por tipo
        otherCard->GetOwner()->TakeDamage(this);
        return mOwner;
    }
    else if (otherCard->HasTypeAdvantageOver(this))
    {
        // A outra carta vence por tipo
        mOwner->TakeDamage(otherCard);
        return otherCard->GetOwner();
    }

    // Se não há vantagem de tipo ou são do mesmo tipo, comparamos valores
    if (mValue > otherCard->GetDamage())
    {
        otherCard->GetOwner()->TakeDamage(this);
        return mOwner;
    }
    else if (otherCard->GetDamage() > mValue)
    {
        mOwner->TakeDamage(otherCard);
        return otherCard->GetOwner();
    }

    // Se houver empate, ninguém recebe dano e retornamos nullptr
    return nullptr;
}