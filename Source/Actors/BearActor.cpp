#include "BearActor.h"
#include "../Combat/Card.h"

BearActor::BearActor(Game* game)
    : AnimatedCharacterActor(game,
        "../Assets/Enemies/Bear/Bear.png",
        "../Assets/Enemies/Bear/Bear.json",
        "../Assets/Enemies/BearAttack/BearAttack.png",
        "../Assets/Enemies/BearAttack/BearAttack.json",
        {0, 1, 2, 3},        // idle frames
        {0, 1, 2, 3},        // attack frames
        {48, 49, 50, 50},    // hurt frames
        {50, 51, 50, 51},    // death frames
        96,   // width
        96)   // height
{
    SetScale(Vector2(-1.0f, 1.0f));
}

BearActor::~BearActor()
{
}

AnimatedCharacterActor::EnemyStats BearActor::GetEnemyStats() const
{
    return {
        28,    // health
        28,    // maxHealth
        0.35   // difficulty (balanceado)
    };
}

std::vector<Card*> BearActor::CreateEnemyDeck() const
{
    std::vector<Card*> deck;
    deck.push_back(new Card("Bear Fire", AttackType::Fire, 4, 2, nullptr));
    deck.push_back(new Card("Bear Neutral Strong", AttackType::Neutral, 5, 3, nullptr));
    deck.push_back(new Card("Bear Plant", AttackType::Plant, 4, 2, nullptr));
    deck.push_back(new Card("Bear Neutral Weak", AttackType::Neutral, 4, 1, nullptr));
    return deck;
}

