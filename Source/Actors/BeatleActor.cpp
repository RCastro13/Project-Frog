#include "BeatleActor.h"

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
