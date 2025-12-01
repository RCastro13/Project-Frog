#include "SceneInputHelper.h"
#include <SDL_keyboard.h>

SceneInputHelper::SceneInputHelper()
{
}

bool SceneInputHelper::IsKeyJustPressed(SDL_Scancode key, const Uint8* keyState)
{
    bool isPressed = keyState[key];
    bool wasPressed = mKeyWasPressed[key];

    if (isPressed && !wasPressed)
    {
        mKeyWasPressed[key] = true;
        return true;
    }
    else if (!isPressed && wasPressed)
    {
        mKeyWasPressed[key] = false;
    }

    return false;
}

void SceneInputHelper::Reset()
{
    mKeyWasPressed.clear();
}

