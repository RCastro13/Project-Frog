#pragma once
#include <SDL_scancode.h>
#include <map>

class SceneInputHelper
{
public:
    SceneInputHelper();

    bool IsKeyJustPressed(SDL_Scancode key, const Uint8* keyState);
    void Reset();

private:
    std::map<SDL_Scancode, bool> mKeyWasPressed;
};

