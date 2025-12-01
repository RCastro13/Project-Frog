#pragma once
#include <string>

// Forward declaration para evitar inclusión circular
class Combatant;

enum class AttackType
{
    Fire,
    Water,
    Plant,
    Neutral
};

class Card {
public:
    Card(std::string name, AttackType type, int value, int coolDown, Combatant* owner = nullptr);

    // Combat
    Combatant* Fight(Card* otherCard);
    bool IsAvailable() const { return mCurrentCooldown <= 0; }
    void StartCooldown() { mCurrentCooldown = mCoolDown; }
    void UpdateCooldown() { if (mCurrentCooldown > 0) mCurrentCooldown--; }

    // Getters
    AttackType GetType() const { return mType; }
    int GetDamage() const { return mValue; }
    int GetCoolDown() const { return mCoolDown - 1; }
    int GetCurrentCooldown() const { return mCurrentCooldown; }
    std::string GetName() const { return mName; }
    Combatant* GetOwner() const { return mOwner; }

    // Setters
    void SetOwner(Combatant* newOwner) { mOwner = newOwner; }

    void ResetCooldown();
    bool HasTypeAdvantageOver(const Card* otherCard) const;
    
private:
    std::string mName;
    AttackType mType;
    int mValue;
    int mCoolDown;
    int mCurrentCooldown;
    Combatant* mOwner;
};