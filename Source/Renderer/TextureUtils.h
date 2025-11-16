#pragma once
#include <SDL.h>

namespace TextureUtils
{
    // Converte uma SDL_Surface para o formato RGBA32
    // Retorna uma nova surface que deve ser liberada com SDL_FreeSurface
    SDL_Surface* ConverterParaRGBA32(SDL_Surface* surf);
}

