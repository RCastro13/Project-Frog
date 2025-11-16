#include "SnakeActor.h"

SnakeActor::SnakeActor(Game* game)
    : AnimatedCharacterActor(game,
        "../Assets/Enemies/Snake/snake.png",
        "../Assets/Enemies/Snake/snake.json",
        "../Assets/Enemies/SnakeAttack/snakeAttack.png",
        "../Assets/Enemies/SnakeAttack/snakeAttack.json",
        {0, 1, 2, 3},        // idle frames
        {0, 1, 2, 3},        // attack frames
        {28, 29, 30, 30},    // hurt frames
        {30, 31, 30, 31})    // death frames
{
    SetScale(Vector2(-1.0f, 1.0f));
}

SnakeActor::~SnakeActor()
{
}
