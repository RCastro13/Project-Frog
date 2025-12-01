//
// Created by kayqu on 20/11/2025.
//

#include "CoinNPC.h"

CoinNPC::CoinNPC(Game* game)
    : NPCActor(game,
               "../Assets/Coin/coin.png",
               "../Assets/Coin/coin.json",
               {0, 1, 2, 3, 4, 5},    // Frames de idle: 6 frames de animação
               90,             // Width (200px original, scaled down)
               105)             // Height (250px original, scaled down)
{
    // Posicionamento será feito pela cena que chamar
}

CoinNPC::~CoinNPC()
{
}