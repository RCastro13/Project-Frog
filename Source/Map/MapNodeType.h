#pragma once

enum class MapNodeType {
    START,      // Nó inicial
    COMBAT,     // Combate normal
    ELITE,      // Combate elite (mais difícil)
    SHOP,       // Loja
    TREASURE,   // Recompensa/Baú
    REST,       // Descanso/Cura
    BOSS        // Boss final
};

