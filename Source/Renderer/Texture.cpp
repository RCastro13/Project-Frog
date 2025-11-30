#include "Texture.h"
#include "TextureUtils.h"
#include <SDL.h>

Texture::Texture()
: mTextureID(0)
, mWidth(0)
, mHeight(0)
{
}

Texture::~Texture()
{
    if (mTextureID != 0)
    {
        glDeleteTextures(1, &mTextureID);
        mTextureID = 0;
    }
}

bool Texture::Load(const std::string &filePath)
{
    SDL_Surface* surf = IMG_Load(filePath.c_str());
    if (!surf) {
        SDL_Log("Falha ao carregar textura: %s", filePath.c_str());
        return false;
    }

    SDL_Surface* formattedSurf = TextureUtils::ConverterParaRGBA32(surf);
    SDL_FreeSurface(surf);

    if (!formattedSurf) {
        return false;
    }

    mWidth = formattedSurf->w;
    mHeight = formattedSurf->h;
    glGenTextures(1, &mTextureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, formattedSurf->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    SDL_FreeSurface(formattedSurf);

    return true;
}

void Texture::CreateFromSurface(SDL_Surface* surf)
{
    if (!surf) {
        SDL_Log("Não é possível criar textura de surface nulo");
        return;
    }

    SDL_Surface* formattedSurf = TextureUtils::ConverterParaRGBA32(surf);
    if (!formattedSurf) {
        return;
    }

    mWidth = formattedSurf->w;
    mHeight = formattedSurf->h;
    glGenTextures(1, &mTextureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, formattedSurf->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_FreeSurface(formattedSurf);
}

void Texture::Unload()
{
	glDeleteTextures(1, &mTextureID);
}

void Texture::SetActive(int index) const
{
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, mTextureID);
}
