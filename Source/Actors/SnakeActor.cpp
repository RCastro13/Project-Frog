#include "SnakeActor.h"
#include "../Combat/Card.h"

SnakeActor::SnakeActor(Game* game)
    : AnimatedCharacterActor(game,
        "../Assets/Enemies/Snake/snake.png",
        "../Assets/Enemies/Snake/snake.json",
        "../Assets/Enemies/SnakeAttack/snakeAttack.png",
        "../Assets/Enemies/SnakeAttack/snakeAttack.json",
        {0, 1, 2, 3},        // idle frames
        {0, 1, 2, 3},        // attack frames
        {28, 29, 30, 30},    // hurt frames
        {30, 31, 30, 31},    // death frames
        96,   // width
        96)   // height
{
    SetScale(Vector2(-1.0f, 1.0f));
}

SnakeActor::~SnakeActor()
{
}

AnimatedCharacterActor::EnemyStats SnakeActor::GetEnemyStats() const
{
    return {
        2,    // health
        28,    // maxHealth
        0.40   // difficulty (rápido e inteligente)
    };
}

std::vector<Card*> SnakeActor::CreateEnemyDeck() const
{
    std::vector<Card*> deck;
    deck.push_back(new Card("Snake Fire", AttackType::Fire, 4, 1, nullptr));
    deck.push_back(new Card("Snake Water", AttackType::Water, 5, 1, nullptr));
    deck.push_back(new Card("Snake Plant", AttackType::Plant, 4, 1, nullptr));
    deck.push_back(new Card("Snake Neutral", AttackType::Neutral, 4, 0, nullptr));
    return deck;
}
