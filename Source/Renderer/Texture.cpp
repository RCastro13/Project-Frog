#include "Texture.h"
#include <SDL.h>

Texture::Texture()
: mTextureID(0)
, mWidth(0)
, mHeight(0)
{
}

Texture::~Texture()
{
}

bool Texture::Load(const std::string &filePath)
{
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
        return false;
    }

    SDL_Surface* surf = IMG_Load(filePath.c_str());
    if (!surf) {
        SDL_Log("Failed to load texture file %s", filePath.c_str());
        return false;
    }

    mWidth = surf->w;
    mHeight = surf->h;
    glGenTextures(1, &mTextureID); // Cria textura na GPU
    glActiveTexture(GL_TEXTURE0); // Coloca textura na região 0 da memória de textura
    glBindTexture(GL_TEXTURE_2D, mTextureID); // Ativa a textura no pipeline gráfico
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, surf->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return true;
}

void Texture::CreateFromSurface(SDL_Surface* surf)
{
    if (!surf) {
        SDL_Log("Cannot create texture from null surface");
        return;
    }

    // Converter surface para formato RGBA32 consistente
    SDL_Surface* formattedSurf = SDL_ConvertSurfaceFormat(surf, SDL_PIXELFORMAT_RGBA32, 0);
    if (!formattedSurf) {
        SDL_Log("Failed to convert surface format: %s", SDL_GetError());
        return;
    }

    mWidth = formattedSurf->w;
    mHeight = formattedSurf->h;
    glGenTextures(1, &mTextureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTextureID);

    // Usar formato RGBA com dados do surface convertido
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, formattedSurf->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Liberar surface convertido
    SDL_FreeSurface(formattedSurf);
}

void Texture::Unload()
{
	glDeleteTextures(1, &mTextureID);
}

void Texture::SetActive(int index) const
{
    glActiveTexture(GL_TEXTURE0 + index); // Coloca textura na região 0 da memória de textura
    glBindTexture(GL_TEXTURE_2D, mTextureID); // Ativa a textura no pipeline gráfico
}
