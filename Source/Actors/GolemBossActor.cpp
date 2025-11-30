#include "GolemBossActor.h"
#include "../Combat/Card.h"

GolemBossActor::GolemBossActor(Game* game)
    : AnimatedCharacterActor(game,
        "../Assets/Enemies/GolemBoss/golem.png",
        "../Assets/Enemies/GolemBoss/golem.json",
        "../Assets/Enemies/GolemBoss/golem.png",  // Usa mesma sprite para ataque
        "../Assets/Enemies/GolemBoss/golem.json",
        {0, 1, 2, 3},              // idle frames
        {20, 21, 22, 23, 24, 25, 26, 27},  // attack frames (8 frames)
        {60, 61, 62, 63},          // hurt frames
        {70, 71, 72, 73, 74, 75, 76, 77},  // death frames (8 frames)
        300,
        300)
{
    SetScale(Vector2(-1.0f, 1.0f));  // Apenas flip horizontal
}

GolemBossActor::~GolemBossActor()
{
}

AnimatedCharacterActor::EnemyStats GolemBossActor::GetEnemyStats() const
{
    return {
        40,    // health (muito mais que inimigos normais)
        40,    // maxHealth
        0.70   // difficulty (muito mais inteligente)
    };
}

std::vector<Card*> GolemBossActor::CreateEnemyDeck() const
{
    std::vector<Card*> deck;
    // Deck mais forte que inimigos normais
    deck.push_back(new Card("Golem Fire", AttackType::Fire, 8, 3, nullptr));
    deck.push_back(new Card("Golem Water", AttackType::Water, 7, 2, nullptr));
    deck.push_back(new Card("Golem Plant", AttackType::Plant, 8, 3, nullptr));
    deck.push_back(new Card("Golem Crush", AttackType::Neutral, 7, 1, nullptr));
    return deck;
}

