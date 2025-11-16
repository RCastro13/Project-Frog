#include "WolfActor.h"

WolfActor::WolfActor(Game* game)
    : AnimatedCharacterActor(game,
        "../Assets/Enemies/Wolf/wolf.png",
        "../Assets/Enemies/Wolf/wolf.json",
        "../Assets/Enemies/WolfAttack/wolfAttack.png",
        "../Assets/Enemies/WolfAttack/wolfAttack.json",
        {0, 1, 2, 3},        // idle frames
        {0, 1, 2, 3},        // attack frames
        {48, 49, 50, 50},    // hurt frames
        {50, 51, 50, 51})    // death frames
{
    SetScale(Vector2(-1.0f, 1.0f));
}

WolfActor::~WolfActor()
{
}
