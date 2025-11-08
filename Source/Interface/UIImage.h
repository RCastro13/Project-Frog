//
// Created by Mi PC on 05/11/2025.
//

#pragma once
#include "../Math.h"
#include <SDL.h>
#include <string>

class UIImage
{
public:
    bool Load(const std::string& fileName);
    void Unload();
    void SetActive(int index = 0);
    int GetWidth() const { return mWidth; }
    int GetHeight() const { return mHeight; }
private:
    SDL_Texture* mTexture;
    Vector2 mPosition;

    int mWidth;
    int mHeight;
};