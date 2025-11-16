#include "RabbitNPC.h"

RabbitNPC::RabbitNPC(Game* game)
    : NPCActor(game,
               "../Assets/Rabbit/rabbit.png",
               "../Assets/Rabbit/rabbit.json",
               {0, 1, 2, 3, 2, 1, 0},    // Frames de idle: 4 frames de animação
               120,             // Width (200px original, scaled down)
               150)             // Height (250px original, scaled down)
{
    // Posicionamento será feito pela ShopScene
}

RabbitNPC::~RabbitNPC()
{
}

