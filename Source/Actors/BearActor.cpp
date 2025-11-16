#include "BearActor.h"

BearActor::BearActor(Game* game)
    : AnimatedCharacterActor(game,
        "../Assets/Enemies/Bear/Bear.png",
        "../Assets/Enemies/Bear/Bear.json",
        "../Assets/Enemies/BearAttack/BearAttack.png",
        "../Assets/Enemies/BearAttack/BearAttack.json",
        {0, 1, 2, 3},        // idle frames
        {0, 1, 2, 3},        // attack frames
        {48, 49, 50, 50},    // hurt frames
        {50, 51, 50, 51})    // death frames
{
    SetScale(Vector2(-1.0f, 1.0f));
}

BearActor::~BearActor()
{
}

