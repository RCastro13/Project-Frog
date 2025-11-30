#include "WolfActor.h"
#include "../Combat/Card.h"

WolfActor::WolfActor(Game* game)
    : AnimatedCharacterActor(game,
        "../Assets/Enemies/Wolf/wolf.png",
        "../Assets/Enemies/Wolf/wolf.json",
        "../Assets/Enemies/WolfAttack/wolfAttack.png",
        "../Assets/Enemies/WolfAttack/wolfAttack.json",
        {0, 1, 2, 3},        // idle frames
        {0, 1, 2, 3},        // attack frames
        {48, 49, 50, 50},    // hurt frames
        {50, 51, 50, 51},    // death frames
        96,   // width
        96)   // height
{
    SetScale(Vector2(-1.0f, 1.0f));
}

WolfActor::~WolfActor()
{
}

AnimatedCharacterActor::EnemyStats WolfActor::GetEnemyStats() const
{
    return {
        2,    // health
        30,    // maxHealth
        0.38   // difficulty (mais forte)
    };
}

std::vector<Card*> WolfActor::CreateEnemyDeck() const
{
    std::vector<Card*> deck;
    deck.push_back(new Card("Wolf Fire Weak", AttackType::Fire, 6, 2, nullptr));
    deck.push_back(new Card("Wolf Fire Strong", AttackType::Fire, 4, 1, nullptr));
    deck.push_back(new Card("Wolf Plant", AttackType::Plant, 5, 1, nullptr));
    deck.push_back(new Card("Wolf Neutral", AttackType::Neutral, 4, 0, nullptr));
    return deck;
}
