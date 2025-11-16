#pragma once

namespace CombatConstants
{
    // Dimensões da tela
    namespace Dimensions
    {
        constexpr float SCREEN_WIDTH = 640.0f;
        constexpr float SCREEN_HEIGHT = 448.0f;
    }

    // Dimensões das cartas
    namespace Cards
    {
        constexpr float SMALL_WIDTH = 80.0f;
        constexpr float SMALL_HEIGHT = 110.0f;
        constexpr float LARGE_WIDTH = 120.0f;
        constexpr float LARGE_HEIGHT = 160.0f;
        constexpr float SPACING = 20.0f;
        constexpr float DISPLAY_SPACING = -8.0f;  // Espaçamento entre cartas no display central
        constexpr float SELECTED_OFFSET = -20.0f;
    }

    // Posições dos atores
    namespace Positions
    {
        constexpr float FROG_X = 150.0f;
        constexpr float FROG_Y = 224.0f;
        constexpr float BEAR_X = 490.0f;
        constexpr float BEAR_Y = 224.0f;
        constexpr float SCREEN_CENTER_X = 320.0f;
        constexpr float SCREEN_CENTER_Y = 224.0f;
        constexpr float CARDS_BASE_Y = 380.0f;
    }

    // Timers
    namespace Timers
    {
        constexpr float ANIMATION_DURATION = 2.0f;
        constexpr float CARD_DISPLAY_DURATION = 2.5f;
        constexpr float PROJECTILE_TRAVEL_TIME = 1.0f;
        constexpr float FADE_DURATION = 0.5f;
    }

    // FPS de animações
    namespace Animation
    {
        constexpr float FROG_FPS = 10.0f;
        constexpr float BEAR_FPS = 10.0f;
        constexpr float PROJECTILE_FPS = 10.0f;
    }

    // Offsets e espaçamentos
    namespace Offsets
    {
        constexpr float POWER_TEXT_FROM_BOTTOM = 30.0f;
        constexpr float POWER_TEXT_FROM_BOTTOM_LARGE = 44.0f;
        constexpr float HP_TEXT_FROM_BAR = 18.0f;
        constexpr float HP_BAR_Y_OFFSET = 160.0f;
        constexpr float WINNER_FRAME_EXTRA = 20.0f;
        constexpr float WINNER_FRAME_Y_OFFSET = -5.0f;
    }

    // Tamanhos
    namespace Sizes
    {
        constexpr float HP_BAR_WIDTH = 120.0f;
        constexpr float HP_BAR_HEIGHT = 12.0f;
        constexpr float HP_BORDER_THICKNESS = 2.0f;
        constexpr float COOLDOWN_ICON_SIZE = 20.0f;
        constexpr float ACTOR_SIZE = 96.0f;
        constexpr float PROJECTILE_SIZE = 64.0f;
    }

    // Tamanhos de fonte
    namespace FontSizes
    {
        constexpr int POWER_SMALL = 24;
        constexpr int POWER_LARGE = 36;
        constexpr int HP_TEXT = 16;
    }
}

