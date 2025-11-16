#include "FrogActor.h"

FrogActor::FrogActor(Game* game)
    : AnimatedCharacterActor(game,
        "../Assets/Frog/Frog.png",
        "../Assets/Frog/Frog.json",
        "../Assets/Frog/Frog.png",
        "../Assets/Frog/Frog.json",
        {0, 1, 2, 3},        // idle frames
        {17, 18, 20, 22},    // attack frames
        {5, 10, 13, 16},     // hurt frames
        {10, 11, 17, 14})    // death frames
{
}

FrogActor::~FrogActor()
{
}

