#include "TextureUtils.h"
#include <SDL_log.h>

namespace TextureUtils
{
    SDL_Surface* ConverterParaRGBA32(SDL_Surface* surf)
    {
        if (!surf) {
            SDL_Log("ERROR: Cannot convert null surface");
            return nullptr;
        }

        SDL_Surface* formattedSurf = SDL_ConvertSurfaceFormat(surf, SDL_PIXELFORMAT_RGBA32, 0);
        if (!formattedSurf) {
            SDL_Log("ERROR: Failed to convert surface format: %s", SDL_GetError());
            return nullptr;
        }

        return formattedSurf;
    }
}

