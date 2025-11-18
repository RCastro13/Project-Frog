#include "BeatleActor.h"
#include "../Combat/Card.h"

BeatleActor::BeatleActor(Game* game)
    : AnimatedCharacterActor(game,
        "../Assets/Enemies/Beatle/beatle.png",
        "../Assets/Enemies/Beatle/beatle.json",
        "../Assets/Enemies/BeatleAttack/beatleAttack.png",
        "../Assets/Enemies/BeatleAttack/beatleAttack.json",
        {0, 1, 2, 3},        // idle frames
        {0, 1, 2, 3},        // attack frames
        {48, 49, 50, 50},    // hurt frames
        {50, 51, 50, 51})    // death frames
{
    SetScale(Vector2(-1.0f, 1.0f));
}

BeatleActor::~BeatleActor()
{
}

AnimatedCharacterActor::EnemyStats BeatleActor::GetEnemyStats() const
{
    return {
        2,    // health
        25,    // maxHealth
        0.30   // difficulty (mais fraco, menos esperto)
    };
}

std::vector<Card*> BeatleActor::CreateEnemyDeck() const
{
    std::vector<Card*> deck;
    deck.push_back(new Card("Beatle Fire", AttackType::Fire, 3, 1, nullptr));
    deck.push_back(new Card("Beatle Water", AttackType::Water, 4, 2, nullptr));
    deck.push_back(new Card("Beatle Plant", AttackType::Plant, 3, 1, nullptr));
    deck.push_back(new Card("Beatle Neutral", AttackType::Neutral, 3, 0, nullptr));
    return deck;
}
